foo:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$foo.size, %esp

# $0 > $1
	movl	$0, %eax
	cmpl	$1, %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)

	movl	-4(%ebp), %eax
	movl	%eax, a

# $1 > $0
	movl	$1, %eax
	cmpl	$0, %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -8(%ebp)

	movl	-8(%ebp), %eax
	movl	%eax, b

# b > a
	movl	b, %eax
	cmpl	a, %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -12(%ebp)

	movl	-12(%ebp), %eax
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