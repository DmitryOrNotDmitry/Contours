#include "CrossFixer.h"

void CrossFixer::calcReplaces(std::vector<std::vector<char>>& matrix, Point XYmin)
{
  int rows = matrix.size();
  if (rows == 0)
    return;

  int cols = matrix[0].size();

  for (int i = 1; i < rows - 1; i++)
  {
    for (int j = 1; j < cols - 1; j++)
    {
      if (
        matrix[i + 1][j - 1] == 0 && matrix[i + 1][j] != 0 && matrix[i + 1][j + 1] == 0 &&
        matrix[i    ][j - 1] != 0 && matrix[i    ][j] != 0 && matrix[i    ][j + 1] != 0 &&
        matrix[i - 1][j - 1] == 0 && matrix[i - 1][j] != 0 && matrix[i - 1][j + 1] == 0
        )
      {
        Point basePoint = Point(j + XYmin.x - 1, i + XYmin.y - 1);
        Point inserted = basePoint.toUp().toRight();

        replaces.push_back(Replacement(basePoint, inserted));
      }
    }
  }
}


bool CrossFixer::skipContour(const Contour& contour, const Replacement& replace) const
{
  int upIdx = contour.indexOf(replace.base.toUp());
  int rightIdx = contour.indexOf(replace.base.toRight());
  return rightIdx == upIdx;
}

