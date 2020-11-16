// un point important � comprendre est qu'il y a le monde 3D et la vue depuis l'�cran
// cela necessite une projection poue adapter la vue depuis l'endroit o� on l'a regarde

// comme mis dans onenote, on notera 3 �tapes:
// travaille avec coordonn�es physique r�el (x,y,z)
// equation de projection et mise en perspective fonction d'une position de l'observateur (X,Y,Z)
// passage en 2D avec drawQuad qui supprime le Z.
// il est possible avec les engins de 3D, on travaille essentiellement avec des triangles et pas des carr�s � voir.



#include <SFML/Graphics.hpp>
using namespace sf;

//int width = 1024;
//int height = 768;


int width = 512;
int height = 384;


int roadW = 2000;
int segL = 200; //segment length
float camD = 0.84; //camera depth


struct Line
{
	float x, y, z; //3d center of line
	float X, Y, W; //screen coord
	float scale, curve;

	Line() { curve = x = y = z = 0; }

	// il est important de comprendre que ces coordonn�es correspondent au point de vue de la camera
	// soit camX, camY et camZ (au point de vue de l'observateur) car la projection effectu�e necessite
	// ces coordonn�es.

	void project(int camX, int camY, int camZ)
	{
		scale = camD / (z - camZ);
		X = (1 + scale * (x - camX)) * width / 2;
		Y = (1 - scale * (y - camY)) * height / 2;
		W = scale * roadW * width / 2;
	}
};

void drawQuad(RenderWindow& w, Color c, int x1, int y1, int w1, int x2, int y2, int w2)
{
	ConvexShape shape(4);
	shape.setFillColor(c);
	shape.setPoint(0, Vector2f(x1 - w1, y1));
	shape.setPoint(1, Vector2f(x2 - w2, y2));
	shape.setPoint(2, Vector2f(x2 + w2, y2));
	shape.setPoint(3, Vector2f(x1 + w1, y1));
	w.draw(shape);
}

int main()
{
	RenderWindow app(VideoMode(width, height), "Outrun Racing!");
	app.setFramerateLimit(60);
	sf::Vector2i Position(0, 0);
	app.setPosition(Position);

	std::vector<Line> lines;

	for (int i = 0; i < 1600; i++)
	{
		Line line;
		line.z = i * segL;

		if (i > 300 && i < 700) line.curve = 0.5;

		if (i > 750) line.y = sin(i / 30.0) * 1500;

		lines.push_back(line);
	}

	int N = lines.size();
	int pos = 0;
	float playerX = 0;

	while (app.isOpen())
	{

		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}


		if (Keyboard::isKeyPressed(Keyboard::Right)) playerX += 200;// deplacement lateral
		if (Keyboard::isKeyPressed(Keyboard::Left)) playerX -= 200;

		if (Keyboard::isKeyPressed(Keyboard::Up)) pos += 200;// avance
		if (Keyboard::isKeyPressed(Keyboard::Down)) pos -= 200; // recule

			//        if (Keyboard::isKeyPressed(Keyboard::Right)) playerX += 0.1;// deplacement lateral
	//        if (Keyboard::isKeyPressed(Keyboard::Left)) playerX -= 0.1;
	//        if (Keyboard::isKeyPressed(Keyboard::Up)) speed = 200;// avance
	//        if (Keyboard::isKeyPressed(Keyboard::Down)) speed = -200; // recule
	//        if (Keyboard::isKeyPressed(Keyboard::Tab)) speed *= 3; // on multiplie la vitesse par trois!! ca marche!! trop cool!
	//        if (Keyboard::isKeyPressed(Keyboard::W)) H += 100;// on prend la hauteur comme si on volait!!!
	//        if (Keyboard::isKeyPressed(Keyboard::S)) H -= 100;// on redescend sur terre !!!



		app.clear();
		int startPos = pos / segL;
		int camH = 1500 + lines[startPos].y;
		float x = 0, dx = 0;
		int maxy = height;


		///////draw road////////

		for (int n = startPos; n < startPos + 300; n++)
		{
			Line& l = lines[n % N];// permet de mani�re subtile de recommencer la boucle quand n atteint 1600 car 1600 % 1600 = 1 (je crois)
			l.project(playerX - x, camH, pos);

			// x (dx � chaque tour) augmente plus vite que dx (0.5 � chaque tour), 
			x += dx;
			dx += l.curve;


			//l.clip = maxy;
			if (l.Y >= maxy) continue;
			maxy = l.Y;

			// mode ternaire finalement pas si illisible que cela: on a une question "?" puis un if/else separ� par un ":"
			// rien de bien sorcier et permet effectivement de gagner de la place.

			Color grass = (n / 3) % 2 ? Color(16, 200, 16) : Color(0, 154, 0);
			Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : Color(0, 0, 0);
			Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);

			if (n > 10)
			{
				Line p = lines[(n - 1) % N]; //previous line

				drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
				drawQuad(app, rumble, p.X, p.Y, p.W * 1.2, l.X, l.Y, l.W * 1.2);
				drawQuad(app, road, p.X, p.Y, p.W, l.X, l.Y, l.W);

			}
		}

		app.display();
	}

	return 0;
}



//
//#include <SFML/Graphics.hpp>
//using namespace sf;
//
//int width = 1024;
//int height = 768;
//int roadW = 2000;
//int segL = 200; //segment length
//float camD = 0.84; //camera depth
//
//
//void drawQuad(RenderWindow& w, Color c, int x1, int y1, int w1, int x2, int y2, int w2)
//{
//    ConvexShape shape(4);
//    shape.setFillColor(c);
//    shape.setPoint(0, Vector2f(x1 - w1, y1));
//    shape.setPoint(1, Vector2f(x2 - w2, y2));
//    shape.setPoint(2, Vector2f(x2 + w2, y2));
//    shape.setPoint(3, Vector2f(x1 + w1, y1));
//    w.draw(shape);
//}
//
//
//// cette structure line semble tr�s importante. elle contient de nombreux param�tres et deux m�thodes.
//
//struct Line
//{
//    float x, y, z; //3d center of line
//    float X, Y, W; //screen coord
//    float curve, spriteX, clip, scale;
//    Sprite sprite;
//
//    Line()
//    {
//        spriteX = curve = x = y = z = 0;
//    }
//
//    void project(int camX, int camY, int camZ)
//    {
//        scale = camD / (z - camZ);
//        X = (1 + scale * (x - camX)) * width / 2;
//        Y = (1 - scale * (y - camY)) * height / 2;
//        W = scale * roadW * width / 2;
//    }
//
//    void drawSprite(RenderWindow& app)
//    {
//        Sprite s = sprite;
//        int w = s.getTextureRect().width;
//        int h = s.getTextureRect().height;
//
//        float destX = X + scale * spriteX * width / 2;
//        float destY = Y + 4;
//        float destW = w * W / 266;
//        float destH = h * W / 266;
//
//        destX += destW * spriteX; //offsetX
//        destY += destH * (-1);    //offsetY
//
//        float clipH = destY + destH - clip;
//        if (clipH < 0) clipH = 0;
//
//        if (clipH >= destH) return;
//        s.setTextureRect(IntRect(0, 0, w, h - h * clipH / destH));
//        s.setScale(destW / w, destH / h);
//        s.setPosition(destX, destY);
//        app.draw(s);
//    }
//};
//
//
//int main()
//{
//    RenderWindow app(VideoMode(width, height), "Outrun Racing!");
//    app.setFramerateLimit(60);
//
//    Texture t[50]; // on initialise un tableau de texture et de sprite
//    Sprite object[50];
//
//    // on charge les images d'arbre, herbes, maisons num�rot� de 1 � 7
//    for (int i = 1; i <= 7; i++)
//    {
//        t[i].loadFromFile("images/" + std::to_string(i) + ".png");// charge la texture depuis le fichier
//        t[i].setSmooth(true);// ameliore la qualit� de la texture? sorte d'antialiasing?
//        object[i].setTexture(t[i]);// met la texture dans le sprite.
//    }
//
//    // charge l'image de fond (768*411)
//    Texture bg;
//    bg.loadFromFile("images/bg.png");
//    bg.setRepeated(true);// quand on depasse la largeur du background, il est simplement repet�
//    Sprite sBackground(bg);
//    sBackground.setTextureRect(IntRect(0, 0, 5000, 411)); // j'imagine que le 5000 sert � g�rer la partie setrepeated et faire comme
//    // si l'image �tait large de 5000 pixel alors qu'elle ne fait que 768 pixel de large.
//    sBackground.setPosition(-2000, 0); // clairement un point que je pige pas avec les position n�gative. on fera des tests.
//
//
//    // on va cr�er un vecteur d'objet line d�fini plus haut. On rentre ici dans du C++ plus int�ressant.
//    // A noter ici que le vecteur de line a un "s", lines ( le vecteur contenant les 1600 lignes).
//    std::vector<Line> lines;
//
//    for (int i = 0; i < 1600; i++)
//    {
//        Line line; // on va cr�er 1600 objet line
//        // g�re probablement la profondeur en z avec un z allant de 0 � 320 000 (1600 *200)
//        line.z = i * segL; // segL : longueur d'un segment fix� � 200
//
//
//        if (i > 300 && i < 700) line.curve = 0.5;
//        if (i > 1100) line.curve = -0.7;
//
//        if (i < 300 && i % 20 == 0) { line.spriteX = -2.5; line.sprite = object[5]; }
//        if (i % 17 == 0) { line.spriteX = 2.0; line.sprite = object[6]; }
//        if (i > 300 && i % 20 == 0) { line.spriteX = -0.7; line.sprite = object[4]; }
//        if (i > 800 && i % 20 == 0) { line.spriteX = -1.2; line.sprite = object[1]; }
//        if (i == 400) { line.spriteX = -1.2; line.sprite = object[7]; }
//
//        if (i > 750) line.y = sin(i / 30.0) * 1500;
//
//        lines.push_back(line);
//    }
//
//    int N = lines.size();
//    float playerX = 0;
//    int pos = 0;
//    int H = 1500;
//
//    while (app.isOpen())
//    {
//        Event e;
//        while (app.pollEvent(e))
//        {
//            if (e.type == Event::Closed)
//                app.close();
//        }
//
//        int speed = 0;
//
//        if (Keyboard::isKeyPressed(Keyboard::Right)) playerX += 0.1;// deplacement lateral
//        if (Keyboard::isKeyPressed(Keyboard::Left)) playerX -= 0.1;
//        if (Keyboard::isKeyPressed(Keyboard::Up)) speed = 200;// avance
//        if (Keyboard::isKeyPressed(Keyboard::Down)) speed = -200; // recule
//        if (Keyboard::isKeyPressed(Keyboard::Tab)) speed *= 3; // on multiplie la vitesse par trois!! ca marche!! trop cool!
//        if (Keyboard::isKeyPressed(Keyboard::W)) H += 100;// on prend la hauteur comme si on volait!!!
//        if (Keyboard::isKeyPressed(Keyboard::S)) H -= 100;// on redescend sur terre !!!
//
//        // increment pos de 200 � chaque it�ration
//        pos += speed;
//        while (pos >= N * segL) pos -= N * segL;
//        while (pos < 0) pos += N * segL;
//
//        app.clear(Color(105, 205, 4));
//        app.draw(sBackground);
//        int startPos = pos / segL;
//        int camH = lines[startPos].y + H; // g�re la hauteur?
//
//        // g�re le fait que l'image de fond se d�cale 
//        if (speed > 0) sBackground.move(-lines[startPos].curve * 2, 0);
//        if (speed < 0) sBackground.move(lines[startPos].curve * 2, 0);
//
//        int maxy = height;
//        float x = 0, dx = 0;
//
//        ///////draw road////////
//        for (int n = startPos; n < startPos + 300; n++)
//        {
//            Line& l = lines[n % N];
//            l.project(playerX * roadW - x, camH, startPos * segL - (n >= N ? N * segL : 0));
//            x += dx;
//            dx += l.curve;
//
//            l.clip = maxy;
//            if (l.Y >= maxy) continue;
//            maxy = l.Y;
//
//            Color grass = (n / 3) % 2 ? Color(16, 200, 16) : Color(0, 154, 0);
//            Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : Color(0, 0, 0);
//            Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);
//
//            Line p = lines[(n - 1) % N]; //previous line
//
//            drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
//            drawQuad(app, rumble, p.X, p.Y, p.W * 1.2, l.X, l.Y, l.W * 1.2);
//            drawQuad(app, road, p.X, p.Y, p.W, l.X, l.Y, l.W);
//        }
//
//        ////////draw objects////////
//        for (int n = startPos + 300; n > startPos; n--)
//            lines[n % N].drawSprite(app);
//
//        app.display();
//    }
//
//    return 0;
//}
