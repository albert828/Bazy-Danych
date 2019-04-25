#define NR_ROOMS 4
#define NR_SENSORS 9
const char *rooms[NR_ROOMS] = {"Pokoj ", "Kuchnia ", "Lazienka ", "Przedpokoj "};
const char *sensors[NR_SENSORS] = {"Temperatura ", "Cisnienie ", "Smog ", "CO2 ", "Wilgotnosc ", "Alkohol ",
                                   "Halas ", "Dym ", "Swiatlo "};
const char *statements[2][NR_SENSORS] = { {"Za goroco otworz okno", "Zbyt wysokie cisnienie", "Duze zanieczyszczenie powietrza, zamknij okno",
                                           "Za duzo CO2 otworz okno", "Za duza wilgotnosc powietrza, uzyj osuszacza", "Jestes pijany!",
                                           "Za glosno, uzyj stoperow", "Wykryto dym, dzwon po straz!", "Za duzo swiatla, ubierz okulary przeciwsloneczne"},
                                           {"Za zimno wlacz ogrzewanie", "Niskie cisnienie", "Powietrze jest ok", "CO2 w normie", "Zbyt niska wilgotnosc, uzyj nawilzacza",
                                           "Jestes trzezwy, brawo ty!", "Jest cichutko", "Nic sie nie pali", "Zbyt ciemno wlacz swiatlo"}};
uint16_t values[9];
bool rand0_1 = 0;

void setup() {
  for(uint8_t i = 0; i < 9; ++i)
    values[i] = 0;
  Serial.begin(115200);
  randomSeed(analogRead(0));
}

void loop() {
  for(uint8_t room_number = 0; room_number < 4; ++room_number)
  {
    for(uint8_t sensor_nr = 0; sensor_nr < NR_SENSORS; ++sensor_nr)
    {
      rand0_1 = random(0,2);
      Serial.println(String("") + rooms[room_number] + " " + sensors[sensor_nr] + " " + values[sensor_nr]
                     + " " + statements[rand0_1][sensor_nr]);
    }
      
    ++values[0]; values[0] %= 40;
    values[1] += 20;
    if(values[1] == 1100)
      values[1] = 900;
    values[2] += 5; values[2] %= 100;
    values[3] += 10; values[3] %= 2000;
    values[4] += 2; values[4] %= 100;
    ++values[5]; values[5] %= 3;
    values[6] += 4; values[6] %= 150;
    ++values[7]; values[8] %= 10;
    values[8] += 50; values[8] %= 2000;
  }
  delay(10000);
}
