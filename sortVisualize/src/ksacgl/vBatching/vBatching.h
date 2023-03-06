typedef struct ksaVertex
{
	float pos[3];
	float sortview_color[3];
	float thickness;
} ksaVertex;

typedef struct ksaQuad
{
	float indices[6];
} ksaQuad;

typedef struct ksaLine
{
	unsigned int indices[2];
} ksaLine;