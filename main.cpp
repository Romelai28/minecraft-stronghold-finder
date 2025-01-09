#include <iostream>
#include <cmath>
#include <string>
#include <sstream>


typedef double CoordinateType;
typedef double AngleType;


struct Point {
	CoordinateType x, y;

	Point(CoordinateType x_ = 0, CoordinateType y_ = 0) : x(x_), y(y_) {}

	Point operator+(const Point& o) const { return {x+o.x, y+o.y}; }
	Point operator-(const Point& o) const { return {x-o.x, y-o.y}; }
	CoordinateType operator*(const Point& o) const { return x*o.x + y*o.y; }  // Dot product
	CoordinateType operator^(const Point& o) const { return x*o.y - y*o.x; }  // Cross product
};


// Scalar multiplication
Point operator*(CoordinateType k, const Point &p) { return {k*p.x, k*p.y}; }


// Print points.
std::ostream &operator <<(std::ostream &os, const Point&p) {
	return os << "(" << p.x << "," << p.y << ")";
}


// Read points.
std::istream &operator >>(std::istream &is, Point&p) {
	return is >> p.x >> p.y;
}


// Convert degrees to radians
AngleType DegreesToRadians(AngleType degrees) {
	return degrees * (M_PI / 180.0);
}


// Before conversion  |  After conversion:
// Z+ ANG: 0          |  Z+ ANG: 0
// x+ ANG: -90        |  x+ ANG: 90
// z- ANG: -180       |  z- ANG: 180
// x- ANG: 90         |  x- ANG: 270
AngleType convertMinecraftDir(AngleType &dir){
	if(dir <= 0){
		return -dir;
	} else {
		return 360-dir;
	}
}


Point getUnitDirectionVector(AngleType directionDeg){
    directionDeg = convertMinecraftDir(directionDeg);
    AngleType directionRad = DegreesToRadians(directionDeg);

    return Point(cos(directionRad), sin(directionRad));
}


// Process a command line to extract the starting point and the direction vector.
// The command lines are of the form: /execute in minecraft:overworld run tp @s X Y Z DIR_HOR DIR_VER
void parseTpCommand(const std::string& input, Point &startingPoint, Point &directionVector) {
    std::stringstream ss(input);
    std::string word;
    int count = 0;

    CoordinateType x, z;
    AngleType directionDeg;

    while (ss >> word) {
        if      (count == 6) {x = std::stof(word);}
        else if (count == 8) {z = std::stof(word);}
        else if (count == 9) {directionDeg = std::stof(word); break;}
        count++;
    }

    // In our minecraft model, the 2D points are of the form (z, x).
    startingPoint = Point(z, x);
    directionVector = getUnitDirectionVector(directionDeg);
}


void readEyeThrow(const std::string &input_message, Point &startingPoint, Point &directionVector){
    std::cout << input_message << "\n";
    std::string input;
    std::getline(std::cin, input);
    parseTpCommand(input, startingPoint, directionVector);
}


// Calculate the intersection between two lines.
// A line is described by a starting point and a direction vector. L = { p + lambda * d, lambda in R}
Point calculateStrongholdPosition(const Point p1, const Point p2, const Point d1, const Point d2){
    // Calculate lambda for line intersection
    CoordinateType lambda = CoordinateType(double((p2 - p1)^d2) / double(d1^d2));

    return p1 + lambda * d1;
}



int main()
{
    Point p1, p2, d1, d2;

	readEyeThrow("First eye: ", p1, d1);
	readEyeThrow("Second eye: ", p2, d2);

	Point stronghold = calculateStrongholdPosition(p1, p2, d1, d2);

	std::cout << "...\n";

	std::cout << "The stronghold most be close to:\n";
	std::cout << "X: " << stronghold.y << " Z: " << stronghold.x;

    return 0;
}
