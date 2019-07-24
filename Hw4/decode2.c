long decode2(long x, long y, long z)
{
	y = y - z;				//subq %rdx, %rsi
	x = x * y;				//imulq %rsi, %rdi
	long return_value;			
	return_value = y;			//movq %rsi, %rax
	return_value = return_value << 63;	//salq $63, %rax
	return_value = return_value >> 63;	//sarq $63, %rax
	return_value = return_value ^ x;	//xorq %rdi, %rax

	return return_value;			//ret

}
