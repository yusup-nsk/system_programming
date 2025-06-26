
void output_bits_of_unsigned_char(unsigned char uch) {
  unsigned char mask_uc;
  for (int i = 7; i >= 0; --i) {
    mask_uc = 1 << i;
    printf("%1d", (uch & mask_uc) >> i);
  }
}

void output_bits_of_int(int num) {
  for (int byte = 3; byte >= 0; --byte) {
    unsigned int uint = (num & (0b11111111 << 8 * byte)) >> 8 * byte;
    unsigned char uch = uint;
    output_bits_of_unsigned_char(uch);
    putchar('`');
  }
}
