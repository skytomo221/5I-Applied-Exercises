(* SIC channel 1 *)
byte smr1: 0xffffb8;
byte brr1: 0xffffb9;
byte scr1: 0xffffba;
byte tdr1: 0xffffbb;
byte ssr1: 0xffffbc;
byte rdr1: 0xffffbd;
word count;
byte c;
byte s;

begin
	scr1 := 0;	(* init *)
	smr1 := 0;	(* async, 8bits, np, stop1 *)
	brr1 := 64;	(* 9600bps *)
	count := 280;
	while count <> 0 do count := count - 1;
	scr1 := 0x30;
	c := ssr1;
	ssr1 := 0x80;

	c := '*';
loop:
	s := ssr1;
	while (s and 0x80) = 0 do s:= ssr1;
	tdr1 := c;
	ssr1 := s and 0x7F;
	
	s := ssr1;
	while (s and 0x78) = 0 do s:= ssr1;
	if (s and 0x40) <> 0 then (* read ok *)
		begin
			c := rdr1;
			ssr1 := s and 0xbf;
		end
	else (*read error *)
		begin
			ssr1 := s and 0xc7;
		end;

	goto loop;
end
