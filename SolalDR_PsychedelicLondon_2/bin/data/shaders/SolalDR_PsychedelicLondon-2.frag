uniform float effectStrength;


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;

    // Time varying pixel color
    //vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));

    
    vec4 noiseR = texture(iChannel1, vec2(uv.x/30., uv.y/30. + iTime/100.));
    vec4 noiseG = texture(iChannel1, vec2(uv.x/20., uv.y/20. + iTime/90.));
    vec4 noiseB = texture(iChannel1, vec2(uv.x/40., uv.y/40. + iTime/80.));

    vec4 imageR = texture(iChannel0, uv + (noiseR.xz/10.)*effectStrength);
    vec4 imageG = texture(iChannel0, uv + (noiseG.xz/20.)*effectStrength);
    vec4 imageB = texture(iChannel0, uv + (noiseB.xz/15.)*effectStrength);
    
    vec3 color = vec3(imageR.x, imageG.y, imageB.z);// + noise.xyz;
    
    // Output to screen
    fragColor = vec4(color.xyz,1.0);
}
