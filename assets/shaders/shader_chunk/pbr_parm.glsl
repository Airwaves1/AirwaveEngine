
struct PBRMaterial
{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;

#ifdef USE_ALBEDO_MAP

    sampler2D albedoMap;

#endif  // USE_ALBEDO_MAP

#ifdef USE_NORMAL_MAP

    sampler2D normalMap;
    
#endif  // USE_NORMAL_MAP

#ifdef USE_METALLIC_ROUGHNESS_MAP

    sampler2D metallicRoughnessMap;
    
#endif  // USE_METALLIC_ROUGHNESS_MAP

#ifdef USE_METALLIC_MAP

    sampler2D metallicMap;
    
#endif // USE_METALLIC_MAP

#ifdef USE_ROUGHNESS_MAP

    sampler2D roughnessMap;

#endif // USE_ROUGHNESS_MAP

#ifdef USE_AO_MAP

    sampler2D aoMap;

#endif // USE_AO_MAP

#ifdef USE_IBL

    samplerCube irradianceMap;

    samplerCube prefilterMap;
    
    sampler2D brdf_lut;

#endif  // USE_IBL

};

uniform PBRMaterial u_material;

