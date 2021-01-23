thickness = 0.8;
screw_diameter = 4;
base_height = 5;

screen_width = 36;
screen_length = 80;
screen_screw_x = (screen_width - 31) / 2;
screen_screw_y = (screen_length - 75) / 2;

pcb_width = 50;
pcb_length = 70;
pcb_screw_x = 2.5;
pcb_screw_y = 3;

pcb_screen_x = 8.5;
pcb_screen_y = 37;
screen_pin_offset_x = 8;
screen_pin_offset_y = 2.5;

screen_screw_sustainer = 10;

$fn=30;

module Screw(diameter=screw_diameter)
{
    circle(d=diameter);
}

module ScreenBase(width=screen_width, length=screen_length, screw_x=screen_screw_x, screw_y=screen_screw_y, add_screws=1)
{
    difference()
    {
        square([length, width]);
        if(add_screws)
        {
            translate([screw_x, screw_y])
            Screw();
            translate([length -screw_x, screw_y])
            Screw();
            translate([screw_x, width - screw_y])
            Screw();
            translate([length -screw_x, width - screw_y])
            Screw();
        }
    }
}

module PCBBase(width=pcb_width, length=pcb_length, screw_x=pcb_screw_x, screw_y=pcb_screw_y, add_screws=1)
{
    difference()
    {
        square([length, width]);
        if(add_screws)
        {
            translate([screw_x, screw_y])
            Screw();
            translate([length -screw_x, screw_y])
            Screw();
            translate([screw_x, width - screw_y])
            Screw();
            translate([length -screw_x, width - screw_y])
            Screw();
        }
    }
}

module Base2d(add_screws=1)
{
    PCBBase(add_screws=add_screws);
    translate([pcb_screen_x - screen_pin_offset_x, pcb_screen_y - screen_pin_offset_y])
    ScreenBase(add_screws=add_screws);
}

module Base()
{
    translate([0, 0, base_height])
    rotate([0, 180, 0])
    {
        linear_extrude(height = base_height)
        difference()
        {
            offset(r=thickness)
            {
                Base2d();
            }
            Base2d(add_screws=0);
        }
        translate([0, 0, base_height - thickness])
        linear_extrude(height = thickness)
        Base2d();
    }
}

module ScrewSustainer()
{
    difference()
    {
        cylinder(d=screw_diameter + 2 * thickness, h=screen_screw_sustainer);
        cylinder(d=screw_diameter, h=screen_screw_sustainer);
    }
}

//Base();
ScrewSustainer();