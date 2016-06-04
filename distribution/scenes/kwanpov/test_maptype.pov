#version 3.7;

box {
  <0,0,0>,<1,1,1>
  pigment {
    image_map {
      png "EarthMap.png"
      map_type planar
    }  
  }
}

sphere {
  <0,0,0>,0.5
  pigment {
    image_map {
      png "EarthMap.png"
      map_type spherical
    }  
  }
  translate <-0.5,0.5,0.5>
}

cylinder {
  <0,0,0>,<0,1,0>,0.5
  pigment {
    image_map {
      png "EarthMap.png"
      map_type cylindrical
    }
  }
  translate <-1.5,0,0.5>
}

torus {
  1,1
  pigment {
    image_map {
      png "EarthMap.png"
      map_type torus
    }
  }
  scale 0.25
  translate <1.5,0.5,0.5>
}

union {
box {
  <0,0,0>,<1,1,1>
  pigment {color rgb <1,1,1>}
  normal {
    bump_map {
      png "EarthMap.png"
      map_type planar
    }  
  }
}

sphere {
  <0,0,0>,0.5
  pigment {color rgb <1,1,1>}
  normal {
    bump_map {
      png "EarthMap.png"
      map_type spherical
    }  
  }
  translate <-0.5,0.5,0.5>
}

cylinder {
  <0,0,0>,<0,1,0>,0.5
  pigment {color rgb <1,1,1>}
  normal {
    bump_map {
      png "EarthMap.png"
      map_type cylindrical
    }
  }
  translate <-1.5,0,0.5>
}

torus {
  1,1
  pigment {color rgb <1,1,1>}
  normal {
    bump_map {
      png "EarthMap.png"
      map_type torus
    }
  }
  scale 0.25
  translate <1.5,0.5,0.5>
}
translate -y
}
light_source {
  <20,20,-20>*1000
  color rgb <1,1,1>
}

camera {
  location <0,2,-5>
  look_at <0,0,0>
  angle 45
}
