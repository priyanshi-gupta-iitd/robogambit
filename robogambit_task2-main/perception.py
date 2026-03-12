import math
import cv2
import numpy as np
import sys


class RoboGambit_Perception:

    def __init__(self):
        # PARAMETERS - Camera intrinsics provided by organisers (DO NOT MODIFY)
        self.camera_matrix = np.array([
            [1030.4890823364258, 0, 960],
            [0, 1030.489103794098, 540],
            [0, 0, 1]
        ], dtype=np.float32)

        self.dist_coeffs = np.zeros((1, 5))

        # INTERNAL VARIABLES
        self.corner_world = {
            21: (350, 350),
            22: (350, -350),
            23: (-350, -350),
            24: (-350, 350)
        }
        self.corner_pixels = {}
        self.pixel_matrix = []
        self.world_matrix = []

        self.H_matrix = None

        self.board = np.zeros((6, 6), dtype=int)

        # ARUCO DETECTOR
        self.aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
        self.aruco_params = cv2.aruco.DetectorParameters()
        self.detector = cv2.aruco.ArucoDetector(self.aruco_dict,self.aruco_params)

        print("Perception Initialized")


    # DO NOT MODIFY THIS FUNCTION
    def prepare_image(self, image):
        """
        DO NOT MODIFY.
        Performs camera undistortion and grayscale conversion.
        """
        undistorted_image = cv2.undistort(image,self.camera_matrix,self.dist_coeffs,None,self.camera_matrix)
        gray_image = cv2.cvtColor(undistorted_image,cv2.COLOR_BGR2GRAY)
        return undistorted_image, gray_image


    # TODO: IMPLEMENT PIXEL → WORLD TRANSFORMATION
    def pixel_to_world(self, pixel_x, pixel_y):
        """
        Convert pixel coordinates into world coordinates using homography.
        Steps:
        1. Ensure homography matrix has been computed.
        2. Format pixel point for cv2.perspectiveTransform().
        3. Return transformed world coordinates.
        """
        if self.H_matrix is None:
            return None, None

        pixel_point = np.array([[[pixel_x, pixel_y]]], dtype=np.float32)
        world_point = cv2.perspectiveTransform(pixel_point, self.H_matrix)

        wx = world_point[0][0][0]
        wy = world_point[0][0][1]
        return wx, wy
    

    # PARTICIPANTS MODIFY THIS FUNCTION
    def process_image(self, image):
        """
        Main perception pipeline.
        Participants must implement:
        - ArUco detection
        - Homography computation
        - Pixel → world conversion
        - Board reconstruction
        """

        self.board[:] = 0

        # Preprocess image (Do not modify)
        undistorted_image, gray_image = self.prepare_image(image)

        # TODO: Detect ArUco markers (uncomment or write your own code)

        # for my understanding lol
        # ids will return flattened list of ids of detected aruco markers
        # corners will be a list which represents shape of each marker like (1,4,2)
        # 1 for contour, 4 for corners of markers, 2 for x,y coords
        corners, ids, rej = self.detector.detectMarkers(gray_image)
        if ids is None:
            return 
        ids = ids.flatten()
        cv2.aruco.drawDetectedMarkers(undistorted_image,corners,ids)


        # TODO: Extract corner marker pixels


        # for my understanding yet again lol
        # loop i -> index, marker_id -> id of detected (possible) markers
        # if id matches the id in given set -> calc mean of x and y coords (for centers)
        # and save it foro each marker
        for i, marker_id in enumerate(ids):
            if marker_id in self.corner_world:
                cx = int(np.mean(corners[i][0][:, 0]))
                cy = int(np.mean(corners[i][0][:, 1]))
                self.corner_pixels[marker_id] = (cx, cy)

        # TODO: Build pixel and world matrices

        # for my understanding yet(yet) again lol
        # basically after recieving corner_pixels, we define matrices
        # we store center coords px, py and then append them
        # we append world coords
        if len(self.corner_pixels) == 4:
            self.pixel_matrix = []
            self.world_matrix = []
            for marker_id, (wx, wy) in self.corner_world.items():
                px, py = self.corner_pixels[marker_id]
                self.pixel_matrix.append([px, py])
                self.world_matrix.append([wx, wy])

        pixel_pts = np.array(self.pixel_matrix, dtype=np.float32)
        world_pts = np.array(self.world_matrix, dtype=np.float32)

        # TODO: Compute homography matrix

        #for my understanding yet(yet(yet)) again lol
        # basically H_matrix is transformation matrix that calculates geometric
        # differences between pixels and world coords
        self.H_matrix, _ = cv2.findHomography(pixel_pts, world_pts)

        # TODO: Convert piece markers to world coordinates

        # for my understanding yet(yet(yet(yet))) again lol
        # this is simple, same process for board pieces.
        for i, marker_id in enumerate(ids):
            if 1<=marker_id<= 10:
                cx = int(np.mean(corners[i][0][:, 0]))
                cy = int(np.mean(corners[i][0][:, 1]))
                wx, wy = self.pixel_to_world(cx, cy)
                if wx is not None:
                    self.place_piece_on_board(marker_id, wx, wy)

        # Visualization (Do not modify)
        res = cv2.resize(undistorted_image, (1152,648))
        cv2.imshow("Detected Markers", res)
        self.visualize_board()


    # TODO: IMPLEMENT BOARD PLACEMENT
    def place_piece_on_board(self, piece_id, x_coord, y_coord):

        """
        Places detected piece on the closest board square.

        Board definition:

        6x6 grid
        top-left corner = (300,300)
        square size = 100mm
        """

        col = int(round(300-x_coord)/100)
        row = int(round(300-y_coord)/100)

        print(f"Piece {piece_id} → row {row}, col {col}")

        if 0<=row<6 and 0<=col<6:
            self.board[row][col] = piece_id


    # DO NOT MODIFY THIS FUNCTION
    def visualize_board(self):
        """
        Draw a simple 6x6 board with detected piece IDs
        """
        cell_size = 80
        board_img = np.ones((6*cell_size,6*cell_size,3),dtype=np.uint8) * 255

        for r in range(6):
            for c in range(6):
                x1 = c*cell_size
                y1 = r*cell_size
                x2 = x1+cell_size
                y2 = y1+cell_size
                cv2.rectangle(board_img,(x1,y1),(x2,y2),(0,0,0),2)

                piece = int(self.board[r][c])
                if piece != 0:
                    cv2.putText(board_img,str(piece),(x1+25,y1+50),cv2.FONT_HERSHEY_SIMPLEX,1,(0,0,255),2)

        cv2.imshow("Game Board", board_img)


# DO NOT MODIFY
def main():
    # To run code, use python/python3 perception.py path/to/image.png
    if len(sys.argv) < 2:
        print("Usage: python perception.py image.png")
        return

    image_path = sys.argv[1]
    image = cv2.imread(image_path)
    if image is None:
        print("Failed to load image")
        return

    perception = RoboGambit_Perception()
    perception.process_image(image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
