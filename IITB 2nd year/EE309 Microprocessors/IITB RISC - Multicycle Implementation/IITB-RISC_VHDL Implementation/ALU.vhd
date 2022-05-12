Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity alu is
    port (
		clk: in std_logic;
        op:    in std_logic_vector(1 downto 0);
        a,b:    in std_logic_vector(15 downto 0);
        result: out std_logic_vector(15 downto 0);   
		out_carry, out_zero: out std_logic
    );
end entity;


architecture alu_logic of alu is 
begin	
	process(a,b,op)
    variable temp:  unsigned(15 downto 0);
	variable temp_a,temp_b,temp_add:  unsigned(16 downto 0);
    begin
		if op="00" then
			temp:=(unsigned(a) NAND unsigned(b));
			if temp="0000000000000000" then
				out_zero<='1';
			else
				out_zero<='0';
			end if;
		elsif op="01" then
			temp:=(unsigned(a) - unsigned(b));
			if temp="0000000000000000" then
				out_zero<='1';
			else
				out_zero<='0';
			end if;
		elsif op="10" then
			temp_a(15 downto 0) := (unsigned(a));
			temp_b(15 downto 0) := (unsigned(b));
			temp_a(16) := '0';
			temp_b(16) := '0';
			temp_add :=  temp_a + temp_b;
			temp:= (temp_add(15 downto 0));
			out_carry <= temp_add(16);
			if temp_add = "0000000000000000" then
				out_zero <= '1';
			else
				out_zero <= '0';
			end if;
		end if;
		result<=std_logic_vector(temp);
	end process;		
end architecture;