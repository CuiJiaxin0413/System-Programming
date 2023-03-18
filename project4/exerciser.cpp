#include "exerciser.h"

void exercise(connection * C) {
  //query1(C, 1, 35, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  query1(C, 1, 35, 40, 1, 15, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  query2(C, "LightBlue");
  query3(C, "UNC");
  query4(C, "NC", "DarkBlue");
 query5(C, 13);
}
