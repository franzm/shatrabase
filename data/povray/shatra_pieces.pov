/***************************************************************************
 *                   Shatra pieces for Povray                              *
 *   (C) 2014 stefan.berke @ modular-audio-graphics.com                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

//+W1000 +H600 +D +P +A0.25 +AM2 +R2 +ua

#version 3.7;

#include "functions.inc"

camera 
{
	orthographic
	
	location  z * -1
	look_at   0
	right     0.96        // TODO: why isn't this 1
	angle     136.5
	translate <2, -1, 0>
}

light_source
{
	<-1,1,-3> * 5
	rgb 1
}

//plane { z, 0 pigment { checker rgb 0.2, rgb 0.4 } }

#declare pieceHeight = 0.2;
#declare ColorDepth = 0.05;


#declare WhiteT = texture
{
	pigment { color rgb 1 }
	finish
	{
		ambient 0.1 diffuse 0.9
		phong 0.7 phong_size 10
	}
	normal { granite 0.3 scale 0.1 }
};

#declare BlackT = texture
{
	finish
	{
		ambient 0.1 diffuse 0.4
		phong 0.7 phong_size 100
	}
	normal { bumps 1 scale 0.02 }
};

#declare White = texture
{
	gradient z
	texture_map
	{
		[0.0 WhiteT] [ColorDepth WhiteT] [ColorDepth+0.01 BlackT]
	}
	translate z*-pieceHeight
};

#declare Black = texture
{
	gradient z
	texture_map
	{
		[0.0 BlackT] [ColorDepth BlackT] [ColorDepth+0.01 WhiteT] 
	}
	translate z*-pieceHeight
};


#declare Gone = texture
{
	pigment { color rgbf <0.8,0.9,1,0.93> }
};

#declare PieceMap = pigment
{
	image_map { png "shatra_pictos.png" interpolate 2 }
};

#declare PieceMapFunc = function { pigment { PieceMap } };

#macro Piece(which)
isosurface
{
	function { -z - PieceMapFunc((x+which-2)/5-0.5,y+0.5,z).x * pieceHeight 
				//+ f_granite(x*10+which,y*10,z) * 0.005 
				}
	contained_by { box { -0.4,<0.4,0.4,-0.01> } }
	threshold 0
	accuracy 1/1000000
	max_gradient 10
}
#end

#declare PieceRow = union
{
	object { Piece(0) }
	object { Piece(1) translate x * 1 }
	object { Piece(2) translate x * 2 }
	object { Piece(3) translate x * 3 }
	object { Piece(4) translate x * 4 }
}

object { PieceRow texture { White } }
object { PieceRow texture { Black } translate y * -1 }
object { PieceRow texture { Gone } translate y * -2 }
	
	

