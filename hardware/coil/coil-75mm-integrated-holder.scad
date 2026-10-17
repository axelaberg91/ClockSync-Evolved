// ATOM Lite / clocksync coil former v6 integrated. Units: mm.
outer_d=75;
core_d=69;
core_wall=1.6;
usable_winding_height=8.8;
lower_flange=1.6;
flare_height=(outer_d-core_d)/2;
upper_flange=1.6;
height=lower_flange+usable_winding_height+flare_height+upper_flange;
pad=34;
pad_corner_r=2;
pad_height=2;
recess_depth=0.6;
recess_clearance=0.15;
pad_offset_y=16.2;
spoke_width=4.5;
spoke_thickness=2;
lead_hole_d=1.8;
resistor_x=23;
resistor_y=-5;
resistor_angle=90;
resistor_pocket_d=3.4;
resistor_mouth=2.5;
resistor_wall=1.2;
$fn=256;
module rounded_pad(){
 linear_extrude(pad_height) offset(r=pad_corner_r,$fn=32)
  square([pad-2*pad_corner_r,pad-2*pad_corner_r],center=true);
}
module ring(){
 difference(){
  union(){
   cylinder(h=height,d=core_d);
   cylinder(h=lower_flange,d=outer_d);
   translate([0,0,lower_flange+usable_winding_height])
    cylinder(h=flare_height,d1=core_d,d2=outer_d);
   translate([0,0,height-upper_flange]) cylinder(h=upper_flange,d=outer_d);
  }
  translate([0,0,-0.1]) cylinder(h=height+0.2,d=core_d-2*core_wall);
 }
}
module placement_recess(){
 translate([0,pad_offset_y,pad_height-recess_depth]) difference(){
  linear_extrude(recess_depth+0.1) offset(r=2.5+recess_clearance,$fn=32)
   square([24,23],center=true);
  translate([-7+recess_clearance,-14.25,-0.1]) cube([14-2*recess_clearance,3.7,recess_depth+0.3]);
 }
}
module resistor_clip(){
 translate([resistor_x,resistor_y,0]) rotate([0,0,resistor_angle]){
  linear_extrude(2) offset(r=1,$fn=32) square([10,6],center=true);
  difference(){
   translate([-2.75,0,4.2]) rotate([0,90,0]) cylinder(h=5.5,r=resistor_pocket_d/2+resistor_wall,$fn=96);
   translate([-2.85,0,4.2]) rotate([0,90,0]) cylinder(h=5.7,d=resistor_pocket_d,$fn=96);
   translate([-3,-resistor_mouth/2,4.2]) cube([6,resistor_mouth,3.8]);
  }
 }
}
union(){
 difference(){
 union(){
  ring();intersection(){translate([0,pad_offset_y,0]) rounded_pad();cylinder(h=pad_height,d=core_d-2*core_wall);}
  resistor_clip();
  for(a=[0,45,90,135]) rotate([0,0,a])
   translate([-core_d/2,-spoke_width/2,0]) cube([core_d,spoke_width,spoke_thickness]);
 }
 translate([0,-(core_d/2-6),3.5])
  rotate([90,0,0]) cylinder(h=11,d=lead_hole_d,$fn=48);

}

translate([0,16.2,1.4]) integrated_atom_holder();
}
module integrated_atom_holder(){
// Dimensions in mm. Generated from build_atom_parts.py.
difference(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){union(){translate([-12.0,-14.0,0]) cube([24.0, 28.0, 1.6]);translate([-14.5,-11.5,0]) cube([29.0, 23.0, 1.6]);}translate([-12.0,-11.5,0]) cylinder(h=1.6,r=2.5,$fn=32);}translate([-12.0,11.5,0]) cylinder(h=1.6,r=2.5,$fn=32);}translate([12.0,-11.5,0]) cylinder(h=1.6,r=2.5,$fn=32);}translate([12.0,11.5,0]) cylinder(h=1.6,r=2.5,$fn=32);}translate([-13.35,-3.5,1.5]) cube([1.1999999999999993, 7.0, 9.857141109999999]);}translate([0,3.5,0]) rotate([90,0,0]) linear_extrude(7.0) polygon([[-12.15, 11.107141109999999], [-11.5, 11.25714111], [-11.5, 11.707141109999998], [-12.6, 12.80714111], [-13.35, 12.80714111], [-13.35, 11.107141109999999]]);}translate([0,3.5,0]) rotate([90,0,0]) linear_extrude(7.0) polygon([[-12.15, 2.6], [-13.35, 2.6], [-14.049999999999999, 1.5], [-12.15, 1.5]]);}translate([-13.35,-10.6,1.5]) cube([1.1999999999999993, 2.5999999999999996, 2.5]);}translate([-10.6,-13.15,1.5]) cube([2.5999999999999996, 1.0, 2.5]);}difference(){translate([-13.35,-13.15,1.5]) cube([3.3499999999999996, 3.1500000000000004, 2.5]);translate([0,0,1.4000000000000001]){union(){union(){union(){union(){union(){translate([-10.15,-12.15,0]) cube([20.3, 24.3, 4.6]);translate([-12.15,-10.15,0]) cube([24.3, 20.3, 4.6]);}translate([-10.15,-10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([-10.15,10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([10.15,-10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([10.15,10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}}}}translate([-13.35,8.0,1.5]) cube([1.1999999999999993, 2.5999999999999996, 2.5]);}translate([-10.6,12.15,1.5]) cube([2.5999999999999996, 1.0, 2.5]);}difference(){translate([-13.35,10.0,1.5]) cube([3.3499999999999996, 3.1500000000000004, 2.5]);translate([0,0,1.4000000000000001]){union(){union(){union(){union(){union(){translate([-10.15,-12.15,0]) cube([20.3, 24.3, 4.6]);translate([-12.15,-10.15,0]) cube([24.3, 20.3, 4.6]);}translate([-10.15,-10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([-10.15,10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([10.15,-10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([10.15,10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}}}}translate([12.15,-3.5,1.5]) cube([1.1999999999999993, 7.0, 9.857141109999999]);}translate([0,3.5,0]) rotate([90,0,0]) linear_extrude(7.0) polygon([[13.35, 11.107141109999999], [13.35, 12.80714111], [12.6, 12.80714111], [11.5, 11.707141109999998], [11.5, 11.25714111], [12.15, 11.107141109999999]]);}translate([0,3.5,0]) rotate([90,0,0]) linear_extrude(7.0) polygon([[12.15, 1.5], [14.049999999999999, 1.5], [13.35, 2.6], [12.15, 2.6]]);}translate([12.15,-10.6,1.5]) cube([1.1999999999999993, 2.5999999999999996, 2.5]);}translate([8.0,-13.15,1.5]) cube([2.5999999999999996, 1.0, 2.5]);}difference(){translate([10.0,-13.15,1.5]) cube([3.3499999999999996, 3.1500000000000004, 2.5]);translate([0,0,1.4000000000000001]){union(){union(){union(){union(){union(){translate([-10.15,-12.15,0]) cube([20.3, 24.3, 4.6]);translate([-12.15,-10.15,0]) cube([24.3, 20.3, 4.6]);}translate([-10.15,-10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([-10.15,10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([10.15,-10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([10.15,10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}}}}translate([12.15,8.0,1.5]) cube([1.1999999999999993, 2.5999999999999996, 2.5]);}translate([8.0,12.15,1.5]) cube([2.5999999999999996, 1.0, 2.5]);}difference(){translate([10.0,10.0,1.5]) cube([3.3499999999999996, 3.1500000000000004, 2.5]);translate([0,0,1.4000000000000001]){union(){union(){union(){union(){union(){translate([-10.15,-12.15,0]) cube([20.3, 24.3, 4.6]);translate([-12.15,-10.15,0]) cube([24.3, 20.3, 4.6]);}translate([-10.15,-10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([-10.15,10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([10.15,-10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}translate([10.15,10.15,0]) cylinder(h=4.6,r=2.0,$fn=32);}}}}translate([-7,-14.1,-0.1]) cube([14, 3.6999999999999993, 1.8000000000000003]);}
}
