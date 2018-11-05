intersection {
  sphere {
    0,1
    scale <1,1,0.5>
  }
  plane {x,0}
  pigment {
    image_map {
      png "EarthMap.png"
      map_type spherical
    }
    rotate x*90
  }
}

intersection {
  sphere {
    0,1
    scale <1,1,0.5>
  }
  plane {-x,0}
  pigment {
    image_map {
      png "EarthMap.png"
      map_type spheroid
      flatness 0.5
    }
    rotate x*90
  }
}

cylinder {
  0,2*x,0.2
  pigment {color rgb x}
}
cylinder {
  0,2*y,0.2
  pigment {color rgb y}
}
cylinder {
  0,2*z,0.2
  pigment {color rgb z}
}

camera {
  sky z
  up y
  right -image_width/image_height*x
  location <0,-5,0>
  look_at <0,0,0>
  angle 45
}

light_source {
  <-20,-20,20>*1000
  color rgb 1
}

