#include <FS.h>
#include <WEMOS_Matrix_LED.h>

MLED mled(0); //intensity=0

// size for 256 chars (8*x8)
char charset[2048];
char scroll_text[] = " ~~~ Jeronimo y Vicente les desean Feliz Navidad. Jo Jo Jo. ~~~ ";
const int SCROLL_LEN = sizeof(scroll_text);
int scroll_text_idx = 0;
int char_bit_idx = 0;
char current_chardef[8] = {};
char bits_scrolled = 0;

void setup() {

  Serial.begin(115200);
  if (!SPIFFS.begin()) {
    Serial.println("FATAL: SPIFFS could not be mounted.");
    return;
  }

  Serial.println("Printing directory...");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
      Serial.print(dir.fileName());
      if(dir.fileSize()) {
          File f = dir.openFile("r");
          Serial.println(f.size());
      }
  }

  File file = SPIFFS.open("/c64-charset.bin", "r");
  if (!file) {
    Serial.println("FATAL: Could not open charset file");
    return;
  }

  size_t bytesRead = file.readBytes(charset, sizeof(charset));
  if (bytesRead != sizeof(charset)) {
    Serial.println("FATAL: could not load charset");
    return;
  }

  Serial.write("Charset loaded Ok!");

  for (int i = 0; i < 8; i++)
  {
    mled.disBuffer[i] = 0;  // clean screen
  }
}

void loop() {
  scroll_left();
  update_right_bit();
  bits_scrolled++;
  if (bits_scrolled == 8) {
    bits_scrolled = 0;
    next_char();
  }
  mled.display();
  delay(60);
}

void next_char() {
  scroll_text_idx++;
  if (scroll_text_idx >= SCROLL_LEN-1)
    scroll_text_idx = 0;

  char offset = scroll_text[scroll_text_idx];
  for (int i=0; i<8; i++) {
    current_chardef[i] = charset[offset * 8 + i];
  }
  Serial.print(offset, HEX);
}

void scroll_left() {
  for (int i=0; i<8; i++) {
    mled.disBuffer[i] <<= 1;
  }
}

void update_right_bit() {
  for (int i=0; i<8; i++) {
    // only right-most bit
    char bit = current_chardef[i] & 128;
    bit >>= 7;
    mled.disBuffer[i] |= bit;
    current_chardef[i] <<= 1;
  }
}