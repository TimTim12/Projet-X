typedef struct s_Point Point;
typedef struct s_ColorRGB ColorRGB;

struct s_Point {
	int x;
	int y;
	struct s_ColorRGB *color;	
};

struct s_ColorRGB {
	int r;
	int g;
	int b;
};
