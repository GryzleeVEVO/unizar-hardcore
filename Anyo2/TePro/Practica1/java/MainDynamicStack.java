import java.util.Iterator;

public class MainDynamicStack
{
	public static void main(String[] args)
	{
		DynamicStack<Integer> stk = new DynamicStack<Integer>();

		stk.push(42);
		stk.push(15);
		stk.pop();
		for (int i=0; i<100; i+=5)
			stk.push(i);

		Iterator<Integer> iter = stk.iterator();
		while(iter.hasNext())
			System.out.print(iter.next()+" ");
		System.out.println();

		// Opcionalmente, con esta nomenclatura, puede hacerse asi
		for (Integer i : stk)
			System.out.print(i+" ");
		System.out.println();
	}
}
