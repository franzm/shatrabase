/***************************************************************************
 * Chess/Shatra pieces for Povray                                          *
 * (C) 2014 stefan.berke @ modular-audio-graphics.com                      *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 ***************************************************************************/

// +W1000 +H600 +A0.1 +AM2 +R5 +ua

#version 3.7;
global_settings { assumed_gamma 1.0 }

#include "transforms.inc"

camera
{
    orthographic
    
    location z * -1
    look_at 0
    right x * 5 / 3
    angle 136.56
    translate <2, -1, 0>
}

light_source { <-1,1,-3> * 50 rgb 1 }
// XXX a negative light color is very experimental ;) 
//light_source { < 1,0.3,-3> * 50 rgb -0.1 }


// ---------- per piece transform -----------

#declare piece_transform = transform 
{
	rotate x * -30
};


// --------------- textures -----------------

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
    pigment { color rgbf <0.5,0.7,1.0, 0.99> }
    finish { Glossy(1) }
};
    
// --------------- helper objects ------------------

#macro SmoothCylinder(y0_, y1_, rad_, smoothness_)
	merge 
	{
		cylinder { y * (y0_ + smoothness_), y * (y1_ - smoothness_), rad_ }
		torus { rad_-smoothness_, smoothness_ translate y * (y0_ + smoothness_) } 
		torus { rad_-smoothness_, smoothness_ translate y * (y1_ - smoothness_) }
	}
#end


// -------------------- pieces ---------------------

#macro Shatra(tex_)
	sor // surface of revolution
	{ 
		12
		<2.8,-0.02>
		<2.8,-0.01>
		<2.8, 0>
		<2.5, 0.5>
		<2, 1>
		<0.7, 3>
		<0.6, 4>
		<1, 5>
		<1.6, 6>
		<1, 6.9>
		<0.1, 6.99>
		<0, 7>
		sturm
		
		texture { tex_ } 
		
		scale 0.86
		translate y*-3.5
		scale 0.8/7
		
		transform { piece_transform }
	}
#end

#macro Tura(tex_)
	merge 
	{ 
		SmoothCylinder(0, 1.2, 2, 0.2)
		cone { y*1.1, 1, y*5, 1.3 }
		difference 
		{
			SmoothCylinder(5,7, 2, 0.4)
			#local cutbox_ = box { <-3, 0, -0.4>, <3, 3, 0.4> translate y*6 }
			object { cutbox_ }
			object { cutbox_ rotate y*40 }
			object { cutbox_ rotate y*90 }
			object { cutbox_ rotate y*140 }
		}
		texture { tex_ } 
		
		translate y*-3.5
		scale 0.8/7
		
		transform { piece_transform }
	}
#end

#macro Yalkyn(tex_)
	difference 
	{ 
		sor 
		{
			10
			<2,-0.02>
			<2,-0.01>
			<2, 0>
			<2, 1>
			<1, 2>
			<1, 4>
			<2, 5>
			<2, 6>
			<0, 7>
			<0, 7>
			sturm
		}
		
		box { <0,0,-3>, <0.5,3,3> rotate z*-30 rotate y*-5 translate <0.3,5.5,0> }
		
		texture { tex_ } 
		
		translate y*-3.5
		scale 0.8/7
		
		transform { piece_transform }
	}
#end


#macro Batyr(tex_)
	merge
	{
		difference 
		{ 
			sor
			{
				9
				<2, 0>
				<2.1, 0.1>
				<3, 2>
				<3.2, 3>
				<3, 4>
				<2, 4.8>
				<1, 4.9>
				<0, 5>
				<0, 5.1>
				sturm
			}
			
			#local cutbox_ = box 
			{ 
				<-1,-1,-5>, <1,1,5> 
				rotate z*45 
				Shear_Trans(<1,0,0>, <0,3,0>, <0,0,1>)
				translate y*6 
			}
			object { cutbox_ }
			object { cutbox_ rotate y*-50 }
			object { cutbox_ rotate y*50 }
			object { cutbox_ rotate y*90 }
		}
		
		torus { 2.2, 0.2 translate y*0.2 }
		
		#local crownsphere_ = sphere { 0, 0.3 scale <1,1.6,1> translate <-1, 5, -2> }
		object { crownsphere_ }
		object { crownsphere_ rotate y*-50 }
		object { crownsphere_ rotate y*130 }
		object { crownsphere_ rotate y*180 }
		
		#local crownsphere_ = sphere { 0, 0.3 scale <1,1.6,1> translate <2.5, 4.5, -1> }
		
		object { crownsphere_ }
		object { crownsphere_ rotate y*-40 }
		object { crownsphere_ rotate y*140 }
		object { crownsphere_ rotate y*180 }
		
		texture { tex_ } 
		
		translate y*-3.5
		scale 0.8/7
		
		transform { piece_transform }
	}
#end


#macro Biy(tex_)
	merge
	{ 
		sor
		{
			9
			<2, 0>
			<2.1, 0.1>
			<3, 2>
			<3.2, 3>
			<3, 4>
			<2, 4.8>
			<1, 4.9>
			<0, 5>
			<0, 5.1>
			sturm
		}
		
		SmoothCylinder(4.8,7,0.3, 0.1)
		object { SmoothCylinder(-1,1, 0.3, 0.1) rotate z*90 translate y*6 }
		
		texture { tex_ } 
		
		translate y*-3.5
		scale 0.8/7
		
		transform { piece_transform }
	}
#end

/* ------------ PUT IT ALL TOGETHER ------------ */
  
#macro PieceRow(tex_)
union
{
    object { Tura   (tex_) translate x * 0 }
    object { Yalkyn (tex_) translate x * 1 }
    object { Batyr  (tex_) translate x * 2 }
    object { Biy    (tex_) translate x * 3 }
    object { Shatra (tex_) translate x * 4 }
}
#end

object { PieceRow(White) }
object { PieceRow(Black) translate y * -1 }
object { PieceRow(Transparent) translate y * -2 }

