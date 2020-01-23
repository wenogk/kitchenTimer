bool compareArrs(long first[5], long second[5]) {
  for (int x = 0; x < 5; x++) {
    if (first[x] != second[x]) {
      return false;
    }
  }
  return true;
}


float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
