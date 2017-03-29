#include "provided.h"
#include "support.h"

bool areEqualCoords(const GeoCoord& a, const  GeoCoord& b){
	return (a.latitude == b.latitude && a.longitude == b.longitude);
}
