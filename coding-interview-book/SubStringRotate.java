
public class SubStringRotate {

    public static boolean isRotation(String a, String b) {
        return a.length() == b.length() && (a + a).contains(b);
    }

    public static void main(String[] arg) {

        String a = "helloworld";
        String b = "worldhello";

        if (isRotation(a, b)) {
            System.out.println("'" + a + "' is a rotation of '" + b + "'");
        } else {
            System.out.println("'" + a + "' is not a rotation of '" + b + "'");
        }

    }
}
