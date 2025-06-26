
void OutputBitsOfUnsignedChar(unsigned char uch) {
  unsigned char mask_uc;
  for (int i = 7; i >= 0; --i) {
    mask_uc = 1 << i;
    printf("%1d", (uch & mask_uc) >> i);
  }
}

void OutputBitsOfInt(int num) {
  for (int byte = 3; byte >= 0; --byte) {
    unsigned int uint = (num & (0xFF << 8 * byte)) >> 8 * byte;
    unsigned char uch = uint;
    OutputBitsOfUnsignedChar(uch);
    putchar('`');
  }
}
