# Contour Definer

## Description

This project implements algorithms for working with raster images that contain arbitrary closed contours without intersections. The project focuses on contour detection, optimization, and the distribution of internal "holes" between the contours.

Stack: C++.

## Main parts of the project

1. **Contour Detection:** The algorithm begins by finding the starting point of the contours, which is determined using an internal point of the contour.

2. **Contour Merging:** The GPC library is used to find "holes" between contours. In complex situations, such as when two contours touch at a single point, an algorithm is proposed to search for and identify such "holes."

3. **Hole Distribution Algorithm:** The algorithm divides a hole into atomic parts, such as squares or triangles, and distributes these parts across neighboring contours based on their shared boundary lengths. It ensures a balanced distribution.

4. **Contour Vector Description Optimization:** To reduce the number of points in a contour, the Douglas-Peucker algorithm is applied. This algorithm is applied while considering common boundaries between contours, ensuring the integrity of the contours.


