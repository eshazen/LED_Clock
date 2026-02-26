//
// hangar for clock
//

mm = 25.4;
e = 0.1;
$fn = 128;

pcb_dia = 9*mm;
plate_dia = 5*mm;
plate_thk = 2;

h_wid = 20;
h_len = 0;

slot_up = 1*mm;

module plate() {
  cylinder( h=plate_thk, d=plate_dia);
}

rotate( [0, 0, 45]) {
  difference() {
    plate();
    for( a=[0:90:270]) {
      rotate( [0, 0, a+45])
	translate( [50, 0, -e])
	cylinder( h=plate_thk+2*e, d=0.188*mm);
    }
  }
}

rotate( [0, 0, -45])
difference() {
  translate( [-h_wid/2, plate_dia/2-5, 0])
    cube( [h_wid, (pcb_dia-plate_dia)/2+h_len, plate_thk]);
  translate( [0, pcb_dia/2+h_len-25, -1]) {
    cylinder( d=10, h=5);
    translate( [0, 10, 0])
      cylinder( d=5, h=5);
    translate( [-2.5, 0, 0])
      cube( [5, 10, 5]);
  }
}

// translate( [0, 0, slot_up-1.6]) color("green") cylinder( h=1.6, d=pcb_dia);
// translate( [0, 0, slot_up-1.6]) % cylinder( h=1.6, d=pcb_dia);
