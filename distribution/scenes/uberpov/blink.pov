// This work is licensed under the Creative Commons Attribution 3.0 Unported License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/
// or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
// California, 94041, USA.
//
// UberPOV Raytracer sample file.
// Created by Christoph Lipka - 2013-08-23
// This file demonstrates the blink keyword.
//
// +w800 +h600 -a
// +w800 +h600 +am2 +a0.1
// +w800 +h600 +am3 +a0.1  +ac0.9  +r3
// +w800 +h600 +am3 +a0.01 +ac0.99 +r6

#version unofficial patch 3.7;
#patch "upov-blink"                0.9;
#patch "upov-reflection-roughness" 0.9;

#declare MOTION_STEPS = 20;

global_settings {
  max_trace_level 25
  assumed_gamma 1.0
}

camera {
  perspective angle 50
  location  <0.0, 3,-5.0>
  right     x*image_width/image_height
  look_at   <0.0, 1.0, 0.0>
}

light_source {
  <3000,3000,0> color rgb 1
//  area_light x*500,y*500, 9,9 adaptive 1 circular orient
}

sky_sphere {
  pigment {
    gradient <0,1,0>
    color_map {
      [0.00 srgb <0.6,0.7,1.0>]
      [0.35 srgb <0.1,0.0,0.8>]
      [0.65 srgb <0.1,0.0,0.8>]
      [1.00 srgb <0.6,0.7,1.0>] 
    } 
    scale 2
  }
}

plane{ <0,1,0>, 0 
  texture{
    pigment{ checker color rgb 1 color rgb 0.1 }
    finish {
      ambient 0.1
      diffuse 0.7
      reflection { 1.0 fresnel roughness 0.005 }
      specular albedo 0.1
      roughness 0.005
    }
  }
  interior { ior 1.5 }
}

sphere { <-1,1,2>, 1
  texture {
    pigment { rgb <0.4, 0.4, 0.5> }
    finish {
      ambient 0.0
      diffuse 0.0
      reflection { 1.0 metallic }
      specular albedo 0.1
      roughness 0.001
      metallic
    }
  }
  interior { ior 1.5 }
}

#declare MovingObject = sphere { <0.5,0.7,0>, 0.7
  texture {
    pigment { rgb <1, 0.8, 0.2> }
    finish {
      ambient 0.1
      diffuse 0.7
      specular albedo 0.1
      roughness 0.005
    }
  }
  interior { ior 1.5 }
}

#declare MOTION_INTERVAL = 1.0/MOTION_STEPS;

#for(I,0.0,1.0-MOTION_INTERVAL,MOTION_INTERVAL)
  object{
    MovingObject
    translate <0.4,0,0.2>*(I+MOTION_INTERVAL/2-0.5)
    blink I, I+MOTION_INTERVAL
  }
#end
