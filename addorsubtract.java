import java.util.Scanner;
public class addorsubtract {
    public static void main(String[] args) {

        Scanner scnr = new Scanner(System.in);

        System.out.print("Enter first operand: ");

        double firstoperand = scnr.nextDouble();

        System.out.print("Enter second operand: ");

        double secondoperand = scnr.nextDouble();

        double result = firstoperand + secondoperand;

        System.out.print("sum" + result);


       double result2 = firstoperand - secondoperand;

       System.out.print("difference" + result2);






    }
}