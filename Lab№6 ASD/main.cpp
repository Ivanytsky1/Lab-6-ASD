#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

// Define a point with x and y coordinates
struct Point {
    int x, y;
};

// Find the point with the lowest y-coordinate (or the leftmost point in case of a tie)
Point p0;

// A utility function to find the next to top in a stack
Point nextToTop(stack<Point>& S) {
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}

// A utility function to swap two points
void swap(Point& p1, Point& p2) {
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

// A utility function to return the square of the distance between p1 and p2
int distSq(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) +
        (p1.y - p2.y) * (p1.y - p2.y);
}

// To find orientation of the ordered triplet (p, q, r).
// The function returns following values:
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) -
        (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // collinear
    return (val > 0) ? 1 : 2; // clock or counterclockwise
}

// A comparison function used by sort() to sort the points with respect to the first point
bool compare(const Point& p1, const Point& p2) {
    int o = orientation(p0, p1, p2);
    if (o == 0)
        return distSq(p0, p2) >= distSq(p0, p1);
    return (o == 2);
}

// Function to return the convex hull of a set of n points
vector<Point> convexHull(vector<Point>& points) {
    int n = points.size();
    // Find the bottom-most point (or choose the left-most point in case of tie)
    int ymin = points[0].y, min = 0;
    for (int i = 1; i < n; i++) {
        int y = points[i].y;
        if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
            ymin = points[i].y, min = i;
    }

    // Place the bottom-most point at the first position
    swap(points[0], points[min]);
    p0 = points[0];

    // Sort n-1 points with respect to the first point
    sort(points.begin() + 1, points.end(), compare);

    // Create an empty stack and push first three points to it
    stack<Point> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);

    // Process remaining n-3 points
    for (int i = 3; i < n; i++) {
        // Keep removing the top while the angle formed by points next-to-top,
        // top, and points[i] makes a non-left turn
        while (orientation(nextToTop(S), S.top(), points[i]) != 2)
            S.pop();
        S.push(points[i]);
    }

    // Store the result in a vector
    vector<Point> hull;
    while (!S.empty()) {
        hull.push_back(S.top());
        S.pop();
    }

    return hull;
}

// Main function
int main() {
    vector<Point> points = { {0, 3}, {1, 1}, {2, 2}, {4, 4}, {0, 0}, {1, 2}, {3, 1}, {3, 3} };
    vector<Point> hull = convexHull(points);

    cout << "The points in the convex hull are: \n";
    for (const auto& point : hull)
        cout << "(" << point.x << ", " << point.y << ")\n";

    return 0;
}
