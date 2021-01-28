void display_num()
{
  switch (number) {
    case 0:
      ugasiSve();
      dis.showNumber(dig[0]);
      upali_cifru(1);
      delay(4);
      delayMicroseconds(500);
    case 1:
      ugasiSve();
      dis.showNumber(dig[1]);
      upali_cifru(2);
      delay(4);
      delayMicroseconds(500);
    case 2:
      ugasiSve();
      dis.showNumber(dig[2]);
      upali_cifru(3);
      delay(4);
      delayMicroseconds(500);
    case 3:
      ugasiSve();
      dis.showNumber(dig[3]);
      upali_cifru(4);
      delay(4);
      delayMicroseconds(500);
    default: ugasiSve();
  }







}
