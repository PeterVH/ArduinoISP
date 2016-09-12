
void avr_start_pmode() {
  SPI.begin();
  digitalWrite(RESET, HIGH);
  pinMode(RESET, OUTPUT);
  digitalWrite(SCK, LOW);
  delay(20);
  digitalWrite(RESET, LOW);
  spi_transaction(0xAC, 0x53, 0x00, 0x00);
  pmode = 1;
}

void avr_end_pmode() {
  SPI.end();
  
  // We're about to take the target out of reset
  // so configure SPI pins as input
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(PIN_SS, INPUT);

  digitalWrite(RESET, HIGH);

  pinMode(RESET, INPUT_PULLUP);
  pmode = 0;
}

void avr_universal() {
  int w;
  uint8_t ch;

  fill(4);
  ch = spi_transaction(buff[0], buff[1], buff[2], buff[3]);
  breply(ch);
}

void flash(uint8_t hilo, int addr, uint8_t data) {
  spi_transaction(0x40+8*hilo, 
  addr>>8 & 0xFF, 
  addr & 0xFF,
  data);
}
void commit(int addr) {
  if (PROG_FLICKER) prog_lamp(LOW);
  spi_transaction(0x4C, (addr >> 8) & 0xFF, addr & 0xFF, 0);
  if (PROG_FLICKER) {
    delay(PTIME);
    prog_lamp(HIGH);
  }
}

uint8_t avr_write_flash_pages(int length) {
  int x = 0;
  int page = current_page(here);
  while (x < length) {
    if (page != current_page(here)) {
      commit(page);
      page = current_page(here);
    }
    flash(LOW, here, buff[x++]);
    flash(HIGH, here, buff[x++]);
    here++;
  }

  commit(page);

  return STK_OK;
}

uint8_t flash_read(uint8_t hilo, int addr) {
  return spi_transaction(0x20 + hilo * 8,
  (addr >> 8) & 0xFF,
  addr & 0xFF,
  0);
}

char avr_flash_read_page(int length) {
  for (int x = 0; x < length; x+=2) {
    uint8_t low = flash_read(LOW, here);
    Serial.print((char) low);
    uint8_t high = flash_read(HIGH, here);
    Serial.print((char) high);
    here++;
  }
  return STK_OK;
}

void avr_read_signature() {
  if (CRC_EOP != getch()) {
    error++;
    Serial.print((char) STK_NOSYNC);
    return;
  }
  Serial.print((char) STK_INSYNC);
  uint8_t high = spi_transaction(0x30, 0x00, 0x00, 0x00);
  Serial.print((char) high);
  uint8_t middle = spi_transaction(0x30, 0x00, 0x01, 0x00);
  Serial.print((char) middle);
  uint8_t low = spi_transaction(0x30, 0x00, 0x02, 0x00);
  Serial.print((char) low);
  Serial.print((char) STK_OK);
}

char avr_eeprom_read_page(int length) {
  // here again we have a word address
  int start = here * 2;
  for (int x = 0; x < length; x++) {
    int addr = start + x;
    uint8_t ee = spi_transaction(0xA0, (addr >> 8) & 0xFF, addr & 0xFF, 0xFF);
    Serial.print((char) ee);
  }
  return STK_OK;
}

// write (length) bytes, (start) is a byte address
uint8_t avr_write_eeprom_chunk(int start, int length) {
  // this writes byte-by-byte,
  // page writing may be faster (4 bytes at a time)
  fill(length);
  prog_lamp(LOW);
  for (int x = 0; x < length; x++) {
    int addr = start+x;
    spi_transaction(0xC0, (addr>>8) & 0xFF, addr & 0xFF, buff[x]);
    delay(45);
  }
  prog_lamp(HIGH); 
  return STK_OK;
}
