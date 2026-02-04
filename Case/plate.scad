
mm = 25.4;
e = 0.1;
$fn = 128;

pcb_dia = 9*mm;
plate_dia = 5*mm;
plate_thk = 2;

clip_height = 0.75*mm;
slot_up = clip_height - 2;
groove_wid = 1.6;
groove_dep = 1.6;
clip_wid = 0.2*mm;

clip_thk = 5;
spoke_wid = 5;

pcb_cor = clip_wid/2-groove_dep;

module clip() {
  rotate( [0, 0, 180])
  translate( [-clip_wid/2, -clip_thk/2, 0]) {
    difference() {
      cube( [clip_wid, clip_thk, clip_height]);
      translate( [clip_wid-groove_dep, -e, slot_up-groove_wid])
	cube( [clip_wid, clip_thk+2*e, groove_wid]);
    }
  }
}

module plate() {
  cylinder( h=plate_thk, d=plate_dia);
  for( a=[0:90:360]) {  
    rotate( [0, 0, a])
      translate( [plate_dia/2-1, -spoke_wid/2, 0])
      cube( [pcb_dia/2+pcb_cor-plate_dia/2+3, spoke_wid, plate_thk]);
  }
}

rotate( [0, 0, 45]) {
  difference() {
    plate();
    for( a=[0:90:360]) {
      rotate( [0, 0, a+45])
	translate( [50, 0, -e])
	cylinder( h=plate_thk+2*e, d=0.188*mm);
    }
  }
  for( a=[0:90:360]) {
    rotate( [0, 0, a])
      translate( [pcb_dia/2+pcb_cor, 0, 0])
      clip();
  }
}

// translate( [0, 0, slot_up-1.6]) color("green") cylinder( h=1.6, d=pcb_dia);
