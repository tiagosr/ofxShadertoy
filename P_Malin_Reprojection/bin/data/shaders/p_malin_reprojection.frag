// Reprojection - @P_Malin

#define kMaxIterations 24

// Turn up iterations if you enable this
//#define ENABLE_SHADOW

const float kPI = 3.141592654;

struct C_Ray
{
    vec3 vOrigin;
    vec3 vDir;
};

vec3 RotateY( const in vec3 vPos, const in float s, const in float c )
{
	vec3 vResult = vec3( c * vPos.x + s * vPos.z, vPos.y, -s * vPos.x + c * vPos.z);

	return vResult;
}

/////////////////////////////////////
// Scene Description 

float GetDistanceBox(const in vec3 vPos, const in vec3 vDimension)
{
	return length(max(abs(vPos)-vDimension,0.0));
}

// result is x=scene distance y=material or object id; zw are material specific parameters (maybe uv co-ordinates)
float GetDistanceScene( const in vec3 vPos )
{   
	float fResult = 1000.0;
	
	float fFloorDist = vPos.y + 3.2;	
	fResult = min(fResult, fFloorDist);
	
	
	vec3 vBuilding1Pos = vec3(58.8, 0.0, 50.0);
	const float fBuilding1Radius = 50.0;
	vec3 vBuilding1Offset = vBuilding1Pos - vPos;
	float fBuilding1Dist = length(vBuilding1Offset.xz) - fBuilding1Radius;
	
	fResult = min(fResult, fBuilding1Dist);
	
	
	
	vec3 vBuilding2Pos = vec3(60.0, 0.0, 55.0);
	const float fBuilding2Radius = 100.0;
	vec3 vBuilding2Offset = vBuilding2Pos - vPos;
	float fBuilding2Dist = length(vBuilding2Offset.xz) - fBuilding2Radius;
	
	fBuilding2Dist = max(vBuilding2Offset.z - 16.0, -fBuilding2Dist); // back only
	
	fResult = min(fResult, fBuilding2Dist);

	
	vec3 vBollardDomain = vPos;
	vBollardDomain -= vec3(1.0, -2.0, 13.5);
	vBollardDomain = RotateY(vBollardDomain, sin(0.6), cos(0.6));
	float fBollardDist = GetDistanceBox(vBollardDomain, vec3(0.2, 1.3, 0.2));
		
	fResult = min(fResult, fBollardDist);

	
	vec3 vFenceDomain = vPos;
	vFenceDomain -= vec3(-5.5, -2.5, 7.0);
	vFenceDomain = RotateY(vFenceDomain, sin(1.0), cos(1.0));
	float fFenceDist = GetDistanceBox(vFenceDomain, vec3(0.5, 1.2, 0.2));
		
	fResult = min(fResult, fFenceDist);
	
	

	vec3 vCabDomain = vPos;
	vCabDomain -= vec3(-1.4, -1.5,29.5);
	vCabDomain = RotateY(vCabDomain, sin(0.01), cos(0.01));
	float fCabDist = GetDistanceBox(vCabDomain, vec3(1.2, 1.5, 3.0));
		
	fResult = min(fResult, fCabDist);
	
		
	vec3 vBusDomain = vPos;
	vBusDomain -= vec3(-15.25, -2.0, 30.0);
	vBusDomain = RotateY(vBusDomain, sin(0.3), cos(0.3));
	float fBusDist = GetDistanceBox(vBusDomain, vec3(1.0, 3.0, 3.0));
		
	fResult = min(fResult, fBusDist);
	
		
	vec3 vBusShelter = vPos;
	vBusShelter -= vec3(7.5, -2.0, 30.0);
	vBusShelter = RotateY(vBusShelter, sin(0.3), cos(0.3));
	float fBusShelterDist = GetDistanceBox(vBusShelter, vec3(1.0, 5.0, 2.0));
		
	fResult = min(fResult, fBusShelterDist);
	
	vec3 vRailings = vPos;
	vRailings -= vec3(12.0, -2.0, 18.0);
	vRailings = RotateY(vRailings, sin(0.3), cos(0.3));
	float fRailings = GetDistanceBox(vRailings, vec3(1.0, 1.0, 2.0));
		
	fResult = min(fResult, fRailings);
		
	
	vec3 vCentralPavement = vPos;
	vCentralPavement -= vec3(5.3, -3.0, 8.0);
	vCentralPavement = RotateY(vCentralPavement, sin(0.6), cos(0.6));
	float fCentralPavementDist = GetDistanceBox(vCentralPavement, vec3(0.8, 0.2, 8.0));
		
	fResult = min(fResult, fCentralPavementDist);
	

	
	return fResult;
}

/////////////////////////////////////
// Raymarching 


// This is an excellent resource on ray marching -> http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float Raymarch( const in C_Ray ray )
{        
    float fDistance = 0.1;
        
    for(int i=0;i<=kMaxIterations;i++)              
    {
        float fSceneDist = GetDistanceScene( ray.vOrigin + ray.vDir * fDistance );
        
        if((fSceneDist <= 0.01) || (fDistance >= 1000.0))
        {
            break;
        }                        

        fDistance = fDistance + fSceneDist; 
    }

	fDistance = min(fDistance, 1000.0);
	
	return fDistance;
}
               

void GetCameraRay( const in vec3 vPos, const in vec3 vForwards, const in vec3 vWorldUp, const in vec2 fragCoord, out C_Ray ray)
{
    vec2 vUV = ( fragCoord.xy / iResolution.xy );
    vec2 vViewCoord = vUV * 2.0 - 1.0;	

	vViewCoord.y *= -1.0;

    ray.vOrigin = vPos;

    vec3 vRight = normalize(cross(vWorldUp, vForwards));
    vec3 vUp = cross(vRight, vForwards);
        
    ray.vDir = normalize( vRight * vViewCoord.x + vUp * vViewCoord.y + vForwards);    
}

void GetCameraRayLookat( const in vec3 vPos, const in vec3 vInterest, const in vec2 fragCoord, out C_Ray ray)
{
	vec3 vForwards = normalize(vInterest - vPos);
	vec3 vUp = vec3(0.0, 1.0, 0.0);

	GetCameraRay(vPos, vForwards, vUp, fragCoord, ray);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    C_Ray ray;

    vec3 vCameraPos = vec3(0.0, 0.0, 0.0);
	
	vCameraPos.x += sin(iGlobalTime * 5.0) * 1.5;
	vCameraPos.z += (sin(iGlobalTime * 3.0) + 1.2) * 3.0;
	
	vec3 vCameraIntrest = vec3(0.0, 1.0, 20.0);

	GetCameraRayLookat( vCameraPos, vCameraIntrest, fragCoord, ray);

    float fHitDist = Raymarch(ray);
	vec3 vHitPos = ray.vOrigin + ray.vDir * fHitDist;
		
	vec3 vProjPos = vHitPos;
		
	float fProjectionDist = 0.5;
	vec2 vUV = vec2(((vProjPos.xy) * fProjectionDist) / vProjPos.z);
	
	vec2 vProjectionOffset = vec2(-0.5, -0.61);
	vUV += vProjectionOffset;
		
	vUV.y = 1.0 - vUV.y;
		
	vec3 vResult = texture2D(iChannel0, vUV).rgb;
	//vec3 vResult = texture2D(iChannel0, fragCoord.xy).rgb;

	if(iMouse.z > 0.0)
	{
		vec3 vGrid =  step(fract(vHitPos / 5.0), vec3(0.9));
		vResult = mix(vec3(1.0, 1.0, 1.0), vResult, vGrid);
	}
	
	#ifdef ENABLE_SHADOW
	C_Ray shadowRay;
	shadowRay.vOrigin = vec3(0.0, 0.0, 0.0);
	shadowRay.vDir = normalize(vHitPos);

	float fLength = length(vHitPos);	
	float fShadowDist = Raymarch(shadowRay);
	
	vResult *= 0.2 + 0.8 * step(fLength, fShadowDist + 0.1);
	#endif
	
	fragColor = vec4(vResult,1.0);
}
