import java.io.*;
import java.net.*;
import java.io.*;
import java.nio.channels.*;


class tcpclient{

	public static void main(String ags[]){
		try{
			SocketChannel sc = SocketChannel.open();
			sc.connect(new InetSocketAddress("127.0.0.1", 9444));
			Console cons = System.console();
			String m = cons.readLine("Enter you message: ");
			ByteBuffer buf = ByteBuffer.wrap(m.getBytes());
			sc.write(buf);
			sc.close();
		}catch(IOExcepion e){
			System.out.println("Got an IO Exception");
		}
	}


}
