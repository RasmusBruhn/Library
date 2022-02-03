#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED

#include <stdinc.h>
#include <SDL2/SDL.h>

// Int point
typedef SDL_Point OBJ_Point;

// int64_t point
struct __OBJ_Point64
{
    int64_t x; // x coordinate
    int64_t y; // y coordinate
};
typedef struct __OBJ_Point64 OBJ_Point64;

// Float point
struct __OBJ_PointF
{
    double x; // x coordinate
    double y; // y coordinate
};
typedef struct __OBJ_PointF OBJ_PointF;

// Int rect
typedef SDL_Rect OBJ_Rect;

// int64_t rect
struct __OBJ_Rect64
{
    int64_t x; // x coordinate of top left corner
    int64_t y; // y coordinate of top left corner
    int64_t w; // Width of rectangle
    int64_t h; // Height of rectangle
};
typedef struct __OBJ_Rect64 OBJ_Rect64;

// Float rect
struct __OBJ_RectF
{
    double x; // x coordinate of top left corner
    double y; // y coordinate of top left corner
    double w; // Width of rectangle
    double h; // Height of rectangle
};
typedef struct __OBJ_RectF OBJ_RectF;

// A circle with an integer center
struct __OBJ_Circle
{
    int32_t x; // x coordinate of center
    int32_t y; // y coordinate of center
    double r; // Radius of circle
};
typedef struct __OBJ_Circle OBJ_Circle;

// A circle with a float center
struct __OBJ_CircleF
{
    double x; // x coordinate of center
    double y; // y coordinate of center
    double r; // Radius of circle
};
typedef struct __OBJ_CircleF OBJ_CircleF;

// An ellipse with integer center x²/a²+y²/b²<=1
struct __OBJ_Ellipse
{
    int32_t x; // x coordinate of center
    int32_t y; // y coordinate of center
    double a; // radius along x, 0 <= a
    double b; // radius along y, 0 <= b
};
typedef struct __OBJ_Ellipse OBJ_Ellipse;

// An ellipse with float center
struct __OBJ_EllipseF
{
    double x; // x coordinate of center
    double y; // y coordinate of center
    double a; // radius along x, 0 <= a
    double b; // radius along y, 0 <= b
};
typedef struct __OBJ_EllipseF OBJ_EllipseF;

// Triangle with integer points
struct __OBJ_Triangle
{
    OBJ_Point p1; // The first point
    OBJ_Point p2; // The second point
    OBJ_Point p3; // The third point
};
typedef struct __OBJ_Triangle OBJ_Triangle;

// Triangle with float points
struct __OBJ_TriangleF
{
    OBJ_PointF p1; // The first point
    OBJ_PointF p2; // The second point
    OBJ_PointF p3; // The third point
};
typedef struct __OBJ_TriangleF OBJ_TriangleF;

// Returns the smallest of x and y
#define OBJ_Min(x, y) (((x) < (y)) ? (x) : (y))

// Returns the biggest of x and y
#define OBJ_Max(x, y) (((x) > (y)) ? (x) : (y))

// Returns true if point is inside of rect
#define OBJ_InRect(Point, Rect) ((Point).x >= (Rect).x && (Point).x < (Rect).x + (Rect).w && (Point).y >= (Rect).y && (Point).y < (Rect).y + (Rect).h)

// Returns true if point is within circle
#define OBJ_InCircle(Point, Circle) ((double)(((Point).x - (Circle).x) * ((Point).x - (Circle).x) + ((Point).y - (Circle).y) * ((Point).y - (Circle).y)) <= (Circle).r * (Circle).r)

// Returns true if point is within ellipse
#define OBJ_InEllipse(Point, Ellipse) ((double)(((Point).x - (Ellipse).x) * ((Point).x - (Ellipse).x)) / ((Ellipse).a * (Ellipse).a) + (double)(((Point).y - (Ellipse).y) * ((Point).y - (Ellipse).y)) / ((Ellipse).b * (Ellipse).b) <= 1.)

// Returns positive if point is to the left of the line, negative when it is to the right and 0 if it is on the line
#define OBJ_InLine(Point, LinePoint1, LinePoint2) (((Point).y - (LinePoint1).y) * ((LinePoint2).x - (LinePoint1).x) - ((LinePoint2).y - (LinePoint1).y) * ((Point).x - (LinePoint1).x))

// Returns true if point is within triangle
#define OBJ_InTriangle(Point, Triangle) (SHP_InLine(Point, (Triangle).p1, (Triangle).p2) >= 0 && SHP_InLine(Point, (Triangle).p2, (Triangle).p3) >= 0 && SHP_InLine(Point, (Triangle).p3, (Triangle).p1) >= 0)

// Changes RectDst to the overlap between the two
#define OBJ_CommonRect(RectDst, RectSrc)                                            \
    (RectDst).w += (RectDst).x;                                                     \
    (RectDst).h += (RectDst).y;                                                     \
    (RectDst).x = OBJ_Max((RectDst).x, (RectSrc).x);                                \
    (RectDst).y = OBJ_Max((RectDst).y, (RectSrc).y);                                \
    (RectDst).w = OBJ_Min((RectDst).w, (RectSrc).x + (RectSrc).w) - (RectDst).x;    \
    (RectDst).h = OBJ_Min((RectDst).h, (RectSrc).y + (RectSrc).h) - (RectDst).y

// Copy rectangle from RectSrc to RectDst
#define OBJ_CopyRect(RectDst, RectSrc) \
    (RectDst).x = (RectSrc).x;         \
    (RectDst).y = (RectSrc).y;         \
    (RectDst).w = (RectSrc).w;         \
    (RectDst).h = (RectSrc).h

// Copy circle from CircleSrc to CircleDst
#define OBJ_CopyCircle(CircleDst, CircleSrc) \
    (CircleDst).x = (CircleSrc).x;           \
    (CircleDst).y = (CircleSrc).y;           \
    (CircleDst).a = (CircleSrc).a;           \
    (CircleDst).b = (CircleSrc).b

// Copy ellipse from EllipseSrc to EllipseDst
#define OBJ_CopyEllipse(EllipseDst, EllipseSrc) \
    (EllipseDst).x = (EllipseSrc).x;            \
    (EllipseDst).y = (EllipseSrc).y;            \
    (EllipseDst).a = (EllipseSrc).a;            \
    (EllipseDst).b = (EllipseSrc).b

// Copy point from PointSrc to PointDst
#define OBJ_CopyPoint(PointDst, PointSrc) \
    (PointDst).x = (PointSrc).x;          \
    (PointDst).y = (PointSrc).y

// Copy triangle from TraingleSrc to TriangleDst
#define OBJ_CopyTriangle(TriangleDst, TraingleSrc)     \
    OBJ_CopyPoint((TriangleDst).p1, (TraingleSrc).p1); \
    OBJ_CopyPoint((TriangleDst).p2, (TraingleSrc).p2); \
    OBJ_CopyPoint((TriangleDst).p3, (TraingleSrc).p3)

// Sets values of rectangle
#define OBJ_SetRect(Rect, _x, _y, _w, _h) \
    (Rect).x = (_x);                      \
    (Rect).y = (_y);                      \
    (Rect).w = (_w);                      \
    (Rect).h = (_h)

// Sets values of circle
#define OBJ_SetCircle(Circle, _x, _y, _r) \
    (Circle).x = (_x);                    \
    (Circle).y = (_y);                    \
    (Circle).r = (_r);

// Sets values of ellipse
#define OBJ_SetEllipse(Ellipse, _x, _y, _a, _b) \
    (Ellipse).x = (_x);                         \
    (Ellipse).y = (_y);                         \
    (Ellipse).a = (_a);                         \
    (Ellipse).b = (_b)

// Sets values of point
#define OBJ_SetPoint(Point, _x, _y) \
    (Point).x = (_x);               \
    (Point).y = (_y)

// Sets values of triangle
#define OBJ_SetTriangle(Triangle, _x1, _y1, _x2, _y2, _x3, _y3) \
    OBJ_SetPoint((Triangle).p1, _x1, _y1);                      \
    OBJ_SetPoint((Triangle).p2, _x2, _y2);                      \
    OBJ_SetPoint((Triangle).p3, _x3, _y3)

// Sets values of triangle from points
#define OBJ_SetTriangleFromPoints(Triangle, Point1, Point2, Point3) OBJ_SetTriangle(Triangle, (Point1).x, (Point1).y, (Point2).x, (Point2).y, (Point3).x, (Point3).y)

// Finds the bounding bow for a circle
#define OBJ_BoundCircle(Rect, Circle)            \
    (Rect).x = (Circle).x - (int32_t)(circle).r; \
    (Rect).y = (Circle).y - (int32_t)(circle).r; \
    (Rect).w = 2 * (int32_t)(circle).r + 1;      \
    (Rect).h = 2 * (int32_t)(circle).r + 1;

// Finds the bounding bow for an ellipse
#define OBJ_BoundEllipse(Rect, Ellipse)            \
    (Rect).x = (Circle).x - (int32_t)(Ellipse).a; \
    (Rect).y = (Circle).y - (int32_t)(Ellipse).b; \
    (Rect).w = 2 * (int32_t)(Ellipse).a + 1;      \
    (Rect).h = 2 * (int32_t)(Ellipse).b + 1;

// Finds the bounding box for a line
#define OBJ_BoundLine(Rect, Point1, Point2)                    \
    (Rect).x = OBJ_Min((Point1).x, (Point2).x);                \
    (Rect).y = OBJ_Min((Point1).y, (Point2).y);                \
    (Rect).w = OBJ_Max((Point1).x, (Point2).x) - (Rect).x + 1; \
    (Rect).h = OBJ_Max((Point1).y, (Point2).y) - (Rect).y + 1;

// Finds the bounding box for a line
#define OBJ_BoundTriangle(Rect, Triangle)                                                          \
    (Rect).x = OBJ_Min((Triangle).p1.x, OBJ_Min((Triangle).p2.x, (Triangle).p3.x));                \
    (Rect).y = OBJ_Min((Triangle).p1.y, OBJ_Min((Triangle).p2.y, (Triangle).p3.y));                \
    (Rect).x = OBJ_Max((Triangle).p1.x, OBJ_Max((Triangle).p2.x, (Triangle).p3.x)) - (Rect).x + 1; \
    (Rect).y = OBJ_Max((Triangle).p1.y, OBJ_Max((Triangle).p2.y, (Triangle).p3.y)) - (Rect).y + 1;

#endif // OBJECTS_H_INCLUDED
