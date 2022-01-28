use <MCAD/servos.scad>
use <servo_arm.scad>

$fn=50;

SERVO = [
    [5, 4, 1, 1.5],
    [25, 0.3, 0.7, 0.1]
];

module parts()
{
    hull()servo_arm(SERVO, [14,1]);
    translate([-19.7,21,-5])
    rotate([90,0,0])
        color("green")import("sensor.stl");
}

module screwholes()
{
    translate([0,0,2])cylinder(r=2,h=12,$fn=50);
    translate([-9.7,22,10.25])rotate([90,0,0]) cylinder(r=1.5, h=7, $fn=50);
    translate([9.8,22,10.25])rotate([90,0,0]) cylinder(r=1.5, h=7, $fn=50);
}

module core()
{
    plate_thickness = 3;
    translate([-4.5,-4,4])cube([9,24,5]);
    translate([-13,20-plate_thickness,4]) cube([26,plate_thickness,8.4]);
    translate([-13,20-plate_thickness,4+8.4]) cube([5,plate_thickness,3]);
    translate([8,20-plate_thickness,4+8.4]) cube([5,plate_thickness,3]);
}

/**
 * taken from https://github.com/openscad/MCAD/blob/master/servos.scad 
 *
 * @param vector position The position vector
 * @param vector rotation The rotation vector
 * @param boolean screws If defined then "screws" will be added and when the module is differenced() from something if will have holes for the screws
 * @param boolean cables If defined then "cables" output will be added and when the module is differenced() from something if will have holes for the cables output
 * @param number axle_length If defined this will draw a red indicator for the main axle
 */
module towerprosg90(position=undef, rotation=undef, screws = 0, axle_length = 0, cables=0)
{
	translate(position) rotate(rotation) {
        difference(){
            union()
            {
                translate([-5.9,-11.8/2,0]) cube([22.5,11.8,22.7]);
                translate([0,0,22.7-0.1]){
                    cylinder(d=11.8,h=4+0.1);
                    hull(){
                        translate([8.8-5/2,0,0]) cylinder(d=5,h=4+0.1);
                        cylinder(d=5,h=4+0.1);
                    }
                    translate([0,0,4]) cylinder(d=4.6,h=3.2);
                }
                translate([-4.7-5.9,-11.8/2,15.9]) cube([22.5+4.7*2, 11.8, 2.5]); 
            }
            //screw holes
            translate([-2.3-5.9,0,15.9+1.25]) cylinder(d=2,h=5, center=true);
            translate([-2.3-5.9-2,0,15.9+1.25]) cube([3,1.3,5], center=true);
            translate([2.3+22.5-5.9,0,15.9+1.25]) cylinder(d=2,h=5, center=true);
            translate([2.3+22.5-5.9+2,0,15.9+1.25]) cube([3,1.3,5], center=true);
        }
        if (axle_length > 0) {
            color("red", 0.3) translate([0,0,29.9/2]) cylinder(r=1, h=29.9+axle_length, center=true);
        }
        if (cables > 0) color("red", 0.3) translate([-12.4,-1.8,4.5]) cube([10,3.6,1.2]);
        if(screws > 0) color("red", 0.3) {
            translate([-2.3-5.9,0,15.9+1.25]) cylinder(d=2,h=10, center=true);
            translate([2.3+22.5-5.9,0,15.9+1.25]) cylinder(d=2,h=10, center=true);
        }
    }
    
}

module sensorholder()
{
    difference()
    {
        core();
        parts();
        screwholes();
    }
}


module largearm()
{
    intersection()
    {
        translate([-25,-0.1,-25])cube([50,50,50]);
        scale([25.4,25.4,25.4]) rotate([0,180,0])import("largearm.stl"); //stl from grabcad
    }

    translate([1.1,0.75,0])cube([33,1.5,2], center=true);
    translate([0,0.75,0]) cube([2,1.5,15], center=true);
    translate([0,0.75,0])rotate([90,0,0]) cylinder(h=1.5, r=2.8, center=true);
}

module servoholder()
{
    difference()
    {
        translate([-10.7,-8,0.9])cube([32,16,15]);
        translate([-0.5,0,0])scale([1.08,1.08,1])towerprosg90([0,0,0], [0,0,0], 1, 0, 0);
        translate([0,-8,10]) scale([1.02,1.02,1.02]) largearm();
    }
}

servoholder();

//towerprosg90([0,0,0], [0,0,0], 1, 0, 0);