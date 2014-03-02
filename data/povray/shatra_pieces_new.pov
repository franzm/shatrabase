/***************************************************************************
 * Shatra pieces for Povray                                                *
 * (C) 2014 stefan.berke @ modular-audio-graphics.com                      *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 ***************************************************************************/

// +W1200 +H600 +A0.1 +AM2 +R5 +ua

#version 3.7
global_settings { assumed_gamma 1.0 }


camera
{
    orthographic
    
    location z * -1
    look_at 0
    right 1.15           // TODO: why isn't this 1
    angle 143.2
    translate <2.5, -1, 0>
}

light_source { <-1,1,-3> * 50 rgb 1 }
// XXX a negative light color is very experimental ;) 
light_source { < 1,0.3,-3> * 50 rgb -0.1 }


#declare pieceHeight = 0.2;     
#declare pieceRadius = 0.42;                
#declare edgeRadius = pieceRadius * 0.1;

#macro Glossy(gloss_)
    phong gloss_ phong_size 30
    specular gloss_ roughness 0.01
#end

#declare White = texture
{
    pigment { color rgb 1 }
    finish { Glossy(1) diffuse 0.6 }
};

#declare WhiteInlay = texture
{
    pigment { color rgb 1 }
    finish { Glossy(2) diffuse 0.25 }
};

#declare Black = texture
{
    pigment { color rgb 0.1 }
    finish { Glossy(1) diffuse 0.2 }
};


#declare Transparent = texture
{ 
    pigment { color rgbf <0.3,0.5,1.0, 0.8> }
    finish { Glossy(1) }
};
    
    

/* basic piece / shatra */
#declare Piece = union
{ 
    //cylinder { 0,z*pieceHeight, pieceRadius }
    sphere { 0,pieceRadius scale <1,1,0.03> translate z*0.03 }
    torus 
    { 
        pieceRadius - edgeRadius, edgeRadius 
        rotate x * 90 
        scale <1,1,0.7>
    }
};                    

/* defunkt piece */
#declare WasPiece = torus
{ 
	pieceRadius - edgeRadius, edgeRadius 
	rotate x * 90 
	scale <1,1,0.7>
};                    


#macro Shatra(White_, Black_)
object { Piece texture { White_ } }
#end


#macro Tura(White_, Black_) 
union
{
    object { Piece texture { White_ } }
    superellipsoid
    {
        <0.15,0.45> 
        scale <0.5, 0.5, 0.1> * pieceRadius
        translate z*0.05
        texture { Black_ }
    } 
}
#end


#macro Yalkyn(White_, Black_)
union
{
    object { Piece texture { White_ } }
    superellipsoid
    {
        <0.25,0.95> 
        scale <0.9, 0.3, 0.03> * pieceRadius
        translate z*0.02
        texture { Black_ }
    }
    /*cylinder
    {
        x * -1, x * 1, 0.4
        scale pieceRadius * 0.85
        scale <1,1,0.1>

        texture { Black }
    }*/ 
}
#end  

#include "shapes3.inc"

#macro Batyr(White_, Black_) 
union
{
    object { Piece texture { White_ } }
    

    smooth_triangle
    {
        #macro trif(ang) 
            <sin(ang*2*pi/3),cos(ang*2*pi/3),0>,
            <sin(ang*2*pi/3),cos(ang*2*pi/3),0>*0.13 - z  
        #end
        trif(0), 
        trif(1), 
        trif(2) 
        
        translate y*-0.1
        scale pieceRadius * 0.63
        
        texture { Black_ }
    }
    
    /*object
    { 
        Facetted_Sphere( 80, 3 )
        scale pieceRadius * 0.5
        rotate x * 90
        texture { Black_ }
    }*/ 
  
}
#end


#macro Biy(White_, Black_) 
union
{
    object { Piece texture { White_ } }
    sphere
    {
        0, pieceRadius * 0.5
        scale <1, 1, 0.1>

        texture { Black_ }
    } 
}
#end
  
#macro PieceRow(White_, Black_)
union
{
    object { Tura   (White_, Black_) translate x * 0 }
    object { Yalkyn (White_, Black_) translate x * 1 }
    object { Batyr  (White_, Black_) translate x * 2 }
    object { Biy    (White_, Black_) translate x * 3 }
    object { Shatra (White_, Black_) translate x * 4 }
    object { Batyr  (White_, Black_) rotate z*180 translate x * 5 }
}
#end

object { PieceRow(White, Black) }
object { PieceRow(Black, WhiteInlay) translate y * -1 }

// wasPiece
union 
{
	object { WasPiece translate x * 0 }
	object { WasPiece translate x * 1 }
	object { WasPiece translate x * 2 }
	object { WasPiece translate x * 4 }
	object { WasPiece translate x * 5 }
	texture { Transparent }
	translate y * -2 
}
