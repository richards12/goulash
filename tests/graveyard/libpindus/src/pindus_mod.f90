module foo
use ISO_C_BINDING

double precision  :: s    ! scalar to be read in

contains

subroutine pindus_setCoeff(s_in) BIND(c,name="pindus_setCoef")
implicit none
real(c_double), intent(in) ::s_in

  s=s_in

end subroutine

subroutine pindus_mult(arr,n ) BIND(c,name="pindus_mult")
implicit none
!type(c_ptr), intent(in) :: arr_in
real(c_double),intent (inout), dimension(n) :: arr 
! call c_f_pointer(arr_in, arr, )
integer(c_int),intent(in)  :: n
integer(c_int) :: i

  !$omp target teams distribute parallel do map(tofrom:arr(1:n))
  do i=1,n
    arr(i)=arr(i) * s 
  end do

end subroutine

end module
