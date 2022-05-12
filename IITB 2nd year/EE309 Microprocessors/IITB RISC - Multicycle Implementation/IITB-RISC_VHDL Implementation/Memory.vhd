library ieee;
use ieee.std_logic_1164.all;
use ieee.math_real.all;
use ieee.numeric_std.all;

entity Memory is
port (	
	rst, clk, write_enabler, read_enabler: in std_logic;
	address, write_data : in std_logic_vector (15 downto 0);
	data_out: out std_logic_vector (15 downto 0)
	);
end entity;

architecture Memory_logic of Memory is
	
type memtype is array(0 to 15) of std_logic_vector(15 downto 0);
signal ram_block : memtype;

begin
    process (write_enabler, read_enabler, address, write_data)
    begin
        -- if (rising_edge(clk)) then
            if rst ='1' then
				ram_block(0) <= "0001010001011000"; 
				ram_block(1) <= "1101010110011101"; 
				ram_block(2) <= "1100010111011001"; 
                ram_block(3) <= "1100110011011001";
				ram_block(4) <= "0001000001010001";
				ram_block(5) <= "0101000001000001";
				ram_block(6) <= "1010101010101010"; 
				ram_block(7) <= "1010101010101010";

				ram_block(8) <= "0010101010101010"; 
				ram_block(9) <= "1000101010101010"; 
				ram_block(10) <= "1010001010101010"; 
				ram_block(11) <= "1010100010101010"; 
				ram_block(12) <= "1010101000101010"; 
				ram_block(13) <= "1010101010001010"; 
				ram_block(14) <= "1010101010100010"; 
				ram_block(15) <= "1010101010101000"; 
                else
                if (write_enabler = '1') then
                    ram_block(to_integer(unsigned(address))) <= write_data;
                end if;
                if (read_enabler = '1') then
                    data_out <= ram_block(to_integer(unsigned(address)));
                end if;
            end if;
        -- end if;
    end process;
end architecture;