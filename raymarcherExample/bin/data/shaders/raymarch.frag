// Created by tiago rezende - tiagosr/2015
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

const float VERY_LARGE_DISTANCE = 100.;
const float CAMERA_DISTORT = .8;
const int MAX_STEPS = 80;
const int MAX_SS_STEPS = 40;

#define pMod(a, b) (mod(a,b)-0.5*b)
#define STANDARD_RAYMARCH
//#define WATCH_DISTANCE
//#define WATCH_STEPS

vec3 opRotX(in float x, in vec3 point) {
    return vec3(point.x, point.y*cos(x)-point.z*sin(x), point.y*sin(x)+point.z*cos(x));
}

mat3 mRotX(in float x) {
    return mat3(
        1.0, 0.0, 0.0,
        0.0, cos(x), -sin(x),
        0.0, sin(x), cos(x)
        );
}

vec3 opRotY(in float y, in vec3 point) {
    return vec3(point.x*cos(y)-point.z*sin(y), point.y, point.x*sin(y)+point.z*cos(y));
}

mat3 mRotY(float y) {
    return mat3(
        cos(y), 0.0, -sin(y),
        0.0, 1.0, 0.0,
        sin(y), 0.0, cos(y)
        );
}

vec3 opRotZ(in float z, in vec3 point) {
    return vec3(point.x*cos(z)-point.y*sin(z), point.x*sin(z)+point.y*cos(z), point.z);
}

mat3 mRotZ(in float z) {
    return mat3(
        cos(z), -sin(z), 0.0,
        sin(z), cos(z), 0.0,
        0.0, 0.0, 1.0
        );
}

vec3 hsv2rgb(in vec3 c)
{
    const vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float fSphere(in vec3 p, in float r) {
    return length(p)-r;
}
float sdTorus(in vec3 p, in vec2 t )
{
    vec2 q = vec2(length(p.xz)-t.x,p.y);
    return length(q)-t.y;
}

float sdBox(in vec3 p, in vec3 b )
{
    vec3 d = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

float sdCone(in vec3 p, in vec3 c) {
    vec2 q = vec2(length(p.xz), p.y);
    float d1 = -p.y-c.z;
    float d2 = max(dot(q, c.xy), p.y);
    return length(max(vec2(d1, d2),0.0)) + min(max(d1, d2), 0.0);
}

float sdCappedCone(in vec3 p, in vec2 c) {
    float q = length(p.xy);
    return dot(c, vec2(q, p.z));
}

float fCylinder(in vec3 p, in float r, in float height) {
    return max(length(p.xz)-r,abs(p.y)-height);
}
float fInfCylinder(in vec2 p, in float r) {
    return length(p)-r;
}

float fCapsule(in vec3 p, in float r, in float c) {
    return mix(length(p.xz)-r,
               length(vec3(p.x,abs(p.y)-c,p.z))-r,
               step(c, abs(p.y)));
}

float fPlane(in vec3 p, in vec3 n) {
    return dot(p, n);
}

vec3 opTx(in vec3 p, in mat4 m) {
    return (inverse(m)*vec4(p,1.0)).xyz;
}



/*
float wSphere(in vec3 center, in float radius, in vec3 pos, in vec3 dir, in float mindist) {
    float r = radius+mindist;
    vec3 cr = cross(center-pos, dir);
    float dist = length(cr);
    if(dist>r) return VERY_LARGE_DISTANCE;
    float ra = sqrt(r*r-dist*dist);
    return length(center-pos+cr)-ra;
}
float wYPlane(in float height, in vec3 pos, in vec3 dir, in float mindist) {
    if (dir.y >=0.) return VERY_LARGE_DISTANCE;
    else return (pos.y+height+mindist)/dir.y;
}
*/
//=====================================================

float map( in vec3 p )
{
    // ground plane
    float ground = p.y + 1.0;
    vec3 v1 = vec3(0.5, 0.5, -0.5)-p;
    vec3 v2 = vec3(0.5, 0.4, -0.5)-p;
    vec3 vt = opRotX(iGlobalTime+v1.x+v1.y, v1);
    float d1 = sdTorus(vec3(pMod(v1.xz, 2.), v1.y).xzy, vec2(0.5,0.1+cos(iGlobalTime*4. + (floor(v1.x/2.0)+floor(v1.z/2.0))*0.5)*0.05));
    float d3 = fSphere(vec3(pMod(v2.xz, 2.), v2.y).xzy, 0.15);
    vec2 p4 = vec2(iGlobalTime*.1,1.2)-p.xy;
    float d4 = fInfCylinder(vec2(pMod(p4.x, 5.), p4.y), 0.5);
    
    return min(ground,min(d1,min(d3, d4)));	
}


float intersect( in vec3 ro, in vec3 rd )
{
	const float maxd = VERY_LARGE_DISTANCE;
	float h = 1.0;
    float t = 0.0;
    for( int i=0; i<MAX_STEPS; i++ )
    {
#ifdef WATCH_STEPS
        if(h<0.001 || t>maxd) return float(i)/float(MAX_STEPS);
#else
        if( h<0.001 || t>maxd ) break;
#endif
	    h = map( ro+rd*t );
        t += h;
    }

    if( t>maxd ) t=-1.0;
	
    return t;
}

vec3 calcNormal( in vec3 pos )
{
    vec3 eps = vec3(0.0002,0.0,0.0);

	return normalize( vec3(
           map(pos+eps.xyy) - map(pos-eps.xyy),
           map(pos+eps.yxy) - map(pos-eps.yxy),
           map(pos+eps.yyx) - map(pos-eps.yyx) ) );
}

float calcSoftshadow( in vec3 ro, in vec3 rd, float k )
{
    float res = 1.0;
    float t = 0.0;
	float h = 1.0;
    for( int i=0; i<MAX_SS_STEPS; i++ )
    {
        //t += max(warpzone(ro+rd*t, rd, k),0.);
        h = map(ro + rd*t);
        res = min( res, k*h/t );
		t += clamp( h, 0.01, 1.0 );
		if( h<0.0001 ) break;
    }
    return clamp(res,0.0,1.0);
}

float calcOcclusion( in vec3 pos, in vec3 nor )
{
    float occ = 0.0;
    float sca = 1.0;
    for( int i=0; i<5; i++ )
    {
        float hr = 0.02 + 0.025*float(i*i);
        vec3 aopos =  nor * hr + pos;
        float dd = map( aopos );
        occ += -(dd-hr)*sca;
        sca *= 0.95;
    }
    return 1.0 - clamp( occ, 0.0, 1.0 );
}

vec3 lig = normalize(vec3(1.0,0.9,0.7));

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 q = fragCoord.xy / iResolution.xy;
    vec2 p = -1.0 + 2.0 * q;
    p.x *= iResolution.x/iResolution.y;

	vec3 ro = (tCameraMatrix*vec4(0., 0., 0., 1.0)).xyz;
	vec3 rd = vec3(p,1.0);
    rd.z -= length(rd)*CAMERA_DISTORT;
    rd = (tCameraMatrix*vec4(normalize(rd), 1.)).xyz - ro;
    vec3 col = vec3(0.0);


#ifdef WATCH_DISTANCE
    float t = intersect(ro,rd);
    fragColor = vec4(vec3(1.-t/VERY_LARGE_DISTANCE),1.);
#endif

#ifdef WATCH_STEPS
    fragColor = vec4(vec3(1.0-intersect(ro, rd)), 1.);
#endif


#ifdef STANDARD_RAYMARCH
    float t = intersect(ro,rd);
    if( t>0.0 )
    {
        vec3 pos = ro + t*rd;
        vec3 nor = calcNormal(pos);
		float sha = calcSoftshadow( pos + nor*0.01, lig, 32.0 );
		float occ = calcOcclusion( pos, nor );
		col =  vec3(0.9,0.6,0.3)*clamp( dot( nor, lig ), 0.0, 1.0 ) * sha;
		col += vec3(0.5,0.6,0.7)*clamp( nor.y, 0.0, 1.0 )*occ;
        col += 0.03;
		col *= exp( -0.2*t );
        col *= 1.0 - smoothstep( 5.0, 10.0, t );
	}

	col = pow( clamp(col,0.0,1.0), vec3(0.45) );
	fragColor = vec4( col, 1.0);
#endif
    //fragColor = vec4( col.x, warpzone(ro,rd)*5., col.z, 1.0 );
}

