#include "pch.h"

#include <vector>
#include <algorithm>

#include "../ContoursDefiner/ContourDefiner.h"
#include "../ContoursDefiner/ContourDefiner.cpp"
#include "../ContoursDefiner/Point.h"
#include "../ContoursDefiner/Point.cpp"
#include "../ContoursDefiner/Contour.h"
#include "../ContoursDefiner/Contour.cpp"
#include "../ContoursDefiner/ImageDataManager.h"
#include "../ContoursDefiner/ImageDataManager.cpp"
#include "../ContoursDefiner/TestImageData.h"
#include "../ContoursDefiner/TestImageData.cpp"
#include "../ContoursDefiner/BresenhamLine.h"
#include "../ContoursDefiner/BresenhamLine.cpp"
#include "../ContoursDefiner/Rect.h"
#include "../ContoursDefiner/Rect.cpp"
#include "../ContoursDefiner/LineBorder.h"
#include "../ContoursDefiner/LineBorder.cpp"
#include "../ContoursDefiner/Path.h"
#include "../ContoursDefiner/Path.cpp"
#include "../ContoursDefiner/GeneralBorderCalculator.h"
#include "../ContoursDefiner/GeneralBorderCalculator.cpp"

#ifdef max
  #undef max
#endif

#ifdef min
  #undef min
#endif

TEST(GeneralBorderCalc, TestAssociationsSort) {
  std::vector<Association> assocs = {
    {8, 10, 1},
    {8, 10, 1},
    {8, 10, 1},
  };

  ASSERT_TRUE(false);
}