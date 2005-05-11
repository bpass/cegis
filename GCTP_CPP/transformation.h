
class Transformation {
public:
	static void transform(Projection* from, Projection* to);
	static void transform(double lon, double lat, Projection* to);
};
