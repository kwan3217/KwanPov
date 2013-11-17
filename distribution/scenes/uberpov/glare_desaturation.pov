// This work is licensed under the Creative Commons Attribution 3.0 Unported License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/
// or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
// California, 94041, USA.
//
// UberPOV Raytracer sample file.
// Created by Christoph Lipka - 2013-08-20
// This file demonstrates the glare desaturation feature
//
// +w640 +h480 +GLD0.0
// +w640 +h480 +GLD0.5
// +w640 +h480 +GLD1.0

#version 3.7;

global_settings {
  assumed_gamma 1.0
}

camera {
  ultra_wide_angle
  right x*image_width/image_height
  location  <0,3,-20>
  look_at   <0,0.5,0>
  angle 20
}

light_source {
  <500,500,-500>       // light's position
  color rgb 2          // light's color
}

sky_sphere {
  pigment {
    gradient y
    color_map {
      [0.0 rgb <0.6,0.7,1.0>]
      [0.7 rgb <0.0,0.1,0.8>]
    }
  }
}

plane {
  y, 0
  texture {
    pigment {
      checker
      color rgb 1
      color rgb 0.5
      scale 0.5
    }
    finish {
      diffuse 0.1
      specular albedo 0.5
      reflection { 1.0 fresnel on }
      conserve_energy
    }
  }
  interior { ior 1.5}
}

sphere {
  <0,1,0>, 1
  texture {
    pigment {rgb <0.5,0.1,1.0>*0.4}
    finish {
      diffuse albedo 0.7
      specular albedo 0.5 roughness 0.01 metallic
    }
  }
}

sphere {
  <-2,1,0>, 1
  texture {
    pigment {rgb <0.5,0.1,1.0>*0.1}
    finish {
      diffuse albedo 0.7
      specular albedo 0.5 roughness 0.01 metallic
    }
  }
}

sphere {
  <2,1,0>, 1
  texture {
    pigment {rgb <0.5,0.1,1.0>}
    finish {
      diffuse albedo 0.7
      specular albedo 0.5 roughness 0.01 metallic
    }
  }
}
