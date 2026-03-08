# include <cstdint>

constexpr uint64_t A_FILE  = 0b000001'000001'000001'000001'000001'000001ULL;
constexpr uint64_t B_FILE  = A_FILE << 1;
constexpr uint64_t AB_FILE = A_FILE | B_FILE;

constexpr uint64_t F_FILE  = 0b100000'100000'100000'100000'100000'100000ULL;
constexpr uint64_t E_FILE  = F_FILE >> 1;
constexpr uint64_t EF_FILE = E_FILE | F_FILE;

constexpr uint64_t NOT_A_FILE  = ~A_FILE;
constexpr uint64_t NOT_AB_FILE = ~AB_FILE;
constexpr uint64_t NOT_F_FILE  = ~F_FILE;
constexpr uint64_t NOT_EF_FILE = ~EF_FILE;

const uint64_t BOARD_MASK = 0xFFFFFFFFFULL;