foo:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$foo.size, %esp
	movl	$1, %eax
	movl	%eax, a
	movl	$2, %eax
	movl	%eax, d

# &a
	leal	a, %eax
	movl	%eax, -4(%ebp)

	movl	-4(%ebp), %eax
	movl	%eax, b
	movl	$0, %eax
	movl	b, %ecx
	movl	%eax, (%ecx)
	movl	%eax, -8(%ebp)

# &d
	leal	d, %eax
	movl	%eax, -12(%ebp)

	movl	-12(%ebp), %eax
	movl	%eax, b

# &b

	movl	b, %eax
	movl	%eax, c
.foo.epilogue:
.L0:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.global	foo
	.set	foo.size, 12

	.data
	.comm	a, 4, 4
	.comm	b, 4, 4
	.comm	c, 4, 4
	.comm	d, 4, 4