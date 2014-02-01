struct vectorJB {
	float x, y, z;
};

void normalize(vectorJB* vec) {
	float length = sqrt(vec->x*vec->x + vec->y*vec->y + vec->z*vec->z);
	vec->x /= length;
	vec->y /= length;
	vec->z /= length;
}

vectorJB Times(vectorJB v1, float value) {
	vectorJB v = { v1.x * value, v1.y * value, v1.z * value };
	return v;
}

vectorJB Add(vectorJB v1, vectorJB v2) {
	vectorJB v = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return v;
}

vectorJB Subtract(vectorJB v1, vectorJB v2) {
	vectorJB v = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return v;
}