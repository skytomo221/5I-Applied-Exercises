byte p4ddr: 0xfee003;
byte p4dr:  0xffffd3;
byte p4pcr: 0xfee03e;
byte p5ddr: 0xfee004;
byte p5dr:  0xffffd4;
byte p5pcr: 0xfee03f;
byte sw_d;
byte led_d;

begin
	p5ddr := 0x00;
	p5pcr := 0xff;
	p4ddr := 0xff;

	led_d := 0x00;

loop:
	sw_d := p5dr;

	if (sw_d and 0x01) = 0x01 then		(* SW1 OFF *)
		begin
			led_d := led_d and 0xbf;	(* LED1 OFF *)
		end
	else
		begin
			led_d := led_d or 0x40;		(* LED1 ON *)
		end;

	if (sw_d and 0x02) = 0x02 then		(* SW2 OFF *)
		begin
			led_d := led_d and 0x7f;	(* LED2 OFF *)
		end
	else
		begin
			led_d := led_d or 0x80;		(* LED2 ON *) 
		end;

	p4dr := led_d;

	goto loop;
end

