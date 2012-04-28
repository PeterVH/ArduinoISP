// The range of at89 device codes is in 0xE0 (through 0xFF?)
// I have not found an official list of device codes for the at89lp mcus.
// The code below is picked arbitrarilly in the at89 range.
// This is ok as long as it matches the value in avrdude.conf
const byte DEVICE_CODE_AT89LP51  0xF1

// ISP commands for the at89lp mcu's:
const byte PREAMBLE = 0xAA;
const byte PREAMBLE2 = 0x55;

const byte PROGRAM_ENABLE = 0xAC;
const byte CHIP_ERASE = 0x8A;
const byte LOAD_CODE_PAGE_BUFFER = 0x51;
const byte WRITE_CODE_PAGE = 0x50;
const byte READ_CODE_PAGE = 0x30;
const byte WRITE_USER_FUSES = 0xE1;
const byte WRITE_USER_FUSES_AUTO_ERASE = 0xF1;
const byte READ_USER_FUSES = 0x61;
const byte WRITE_LOCK_BITS = 0xE4;
const byte READ_LOCK_BITS = 0x66;
const byte READ_ATMEL_SIGNATURE = 0x38;

const byte PROGRAM_ENABLE_ADDR_HI = 0x53;

static unsigned char buffer[32];

byte at89_isp_read(byte opcode, byte addr_hi, byte addr_lo, int bytesToRead ) {
  byte reply = 0;
  int i;

  // take the slave select low to select the device:
  digitalWrite(SS, LOW);
  
  delay(10);

  SPI.transfer(PREAMBLE);

  if (param.devicecode == 0xF1)
    SPI.transfer(PREAMBLE2);

  SPI.transfer(opcode);
  SPI.transfer(addr_hi);
  SPI.transfer(addr_lo);

  for(i=0; i<bytesToRead; i++) {
    reply = SPI.transfer(0x00);
    buffer[i] = reply;

    TRACE2(reply, HEX);
    if(i%16==15)
      TRACELN("");
    else
      TRACE(" ");
  }

  // take the slave select high to de-select:
  digitalWrite(SS, HIGH);

  return reply;
}

void at89_isp_write(byte opcode, byte addr_hi, byte addr_lo,
                  const unsigned char* p, int bytesToWrite ) {
  byte reply;
  int i;

  // take the slave select low to select the device:
  digitalWrite(SS, LOW);
  
  delay(10);

  SPI.transfer(PREAMBLE);
  if (param.devicecode == 0xF1)
    SPI.transfer(PREAMBLE2);
  SPI.transfer(opcode);
  SPI.transfer(addr_hi);
  SPI.transfer(addr_lo);

  for(i=0; i<bytesToWrite; i++) {
    SPI.transfer(p[i]);

    TRACE2(p[i], HEX);
    if(i%16==15)
      TRACELN("");
    else
      TRACE(" ");
  }

  // take the slave select high to de-select:
  digitalWrite(SS, HIGH);
}

void at89_start_pmode() {
  
  TRACELN("at89 enter pmode");
  
  // keep slave select high,  
  // it will be pulled low during each spi transfer 
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  
  // start the SPI library:
  SPI.begin();

  // reset target
  digitalWrite(RESET_LP, HIGH);

  delay(1000);

  at89_isp_read(PROGRAM_ENABLE, PROGRAM_ENABLE_ADDR_HI, 0, 0); 

  pmode = 1;  
}

void at89_universal() {  
  
  int w;
  uint8_t opcode, addr_hi, addr_lo, ch;

  opcode = getch();
  addr_hi = getch();
  addr_lo = getch();
  ch = getch();

  TRACE2(opcode, HEX);TRACE(" ");
  TRACE2(addr_hi, HEX);TRACE(" ");
  TRACE2(addr_lo, HEX);TRACE(" ");
  TRACE2(ch, HEX);TRACE(" ");
  TRACELN("");

  ch = at89_isp_read(opcode, addr_hi, addr_lo, 1);
  breply(ch);
}

char at89_flash_read_page(int length) {
  unsigned char addrLo = here & 0xFF;
  unsigned char addrHi = (here & 0xF00) >> 8;
  at89_isp_read(READ_CODE_PAGE, addrHi, addrLo, 32);

  for (int i = 0; i < length; i++) {
    Serial.print((char) buffer[i]);
  }
  return STK_OK;
}

uint8_t at89_write_flash_pages(int length) {

  unsigned char addrLo = here & 0xFF;
  unsigned char addrHi = (here & 0xF00) >> 8;
  //TRACE("@"); TRACE2(here, HEX);
  //TRACE(", l="); TRACELN(length);
  at89_isp_write(WRITE_CODE_PAGE, addrHi, addrLo, buff, length);

  return STK_OK;
}

void at89_read_signature()
{
  // not needed: avrdude uses universal for this
  TRACE("not yet implemented");
}

uint8_t at89_write_eeprom_chunk(int start, int length)
{
  TRACE("not yet implemented");
  return STK_FAILED;
}

char at89_eeprom_read_page(int length)
{
  TRACE("not yet implemented");
  return STK_FAILED;
}
