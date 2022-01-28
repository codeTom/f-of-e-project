use <servoholder.scad>;
use <servo_arm.scad>

SERVO = [
    [5, 4, 1, 1.5],
    [25, 0.3, 0.7, 0.1]
];

module servo()
{
    translate([0,-51,0])color("lightblue") import("servo.stl");
}

module sensor()
{
    color("green")import("sensor.stl");
}
servo();
translate([10,10,23])rotate([90,0,0])
{
    color("white")translate([0,-9,10])scale([25.4,25.4,25.4]) rotate([0,180,0])import("largearm.stl");
    servoholder();
}
translate([0,-6,23]) rotate([90,0,0]) servo();
color("white")translate([10,-18,23])rotate([90,55,0])servo_standard(13, 1);
//translate([35,-10,0])rotate([-25,0,-90])rotate([0,90,0])sensor();

translate([9,-19,22.5])rotate([90,55,0]){
    sensorholder();
translate([-19.7,21,-5])
    rotate([90,0,0])
        color("green")import("sensor.stl");
}