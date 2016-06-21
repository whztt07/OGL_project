/* 
	Данный код является результатом уроков по OpenGL. Он не связан с проектом напрямую, но помогает понять базовые принципы работы с OpenGL.
	Так как код является по большей части наглядным пособием, он изобилует комментариями разной степенью очевидности, вплоть до "это и ежу ясно". Порою.
	А порою комментарии отстутствуют там, где они очень нужны и "без поллитры не разберешься". 
	Я предупредил.
	Оригинальный сайт с уроками: http://ogldev.atspace.co.uk/index.html
	Переводы: http://triplepointfive.github.io/ogltutor/
*/

#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include "glew.h" /* 
						Обеспечивает эффективные run-time механизмы для определения того, какие OpenGL расширения поддерживаются на целевой платформе. 
						Если вы подключаете другие заголовки OpenGL, вам следует подключить его раньше остальных, иначе GLEW откажется работать. 
					*/
#include "freeglut.h" /* 
						API для управления оконной системой, а так же обработка событий, контроль ввода/вывода и ещё несколько других возможностей 
					*/

#include "pipeline.h" /* 
						Кастомный заголовок, постепенно увеличивающийся в ходе обучения 
					*/

#pragma comment( user, "Compiled on " __DATE__ " at " __TIME__ " with") /*
						Строка "Compiled on ДАТА-КОМПИЛЯЦИИ at ВРЕМЯ-КОМПИЛЯЦИИ" будет записана в ЕХЕ файл
						Ни на что влиять не будет, но будет видна в ЕХЕшнике в виде текста.
					*/
#pragma comment( compiler ) /*
						В ЕХЕшник будет записано имя и версия компилятора. 
						Ни на что не влияет, но можно будет потом посмотреть.
					*/
					
GLuint VBO; // Глобальная переменная для хранения указателя на буфер вершин
GLuint IBO; // То же самое для буфера индексов этих вершин
GLuint gWorldLocation; /*
							Мы используем этот указатель для доступа к всемирной матрице, 
							представленной в виде uniform-переменной внутри шейдера. 
							Всемирная она потому, что всё что мы делаем с объектом, это изменение его позиции в место, 
							которое мы указываем относительно координатной системы внутри нашего виртуального ‘мира’.
					   */

/*
	Из-за технических трудностей нижеследующий код шейдеров закомментировать не удалось.
	Тем не менее, общая информация по синтаксису успешно гуглится (нет), так что не все так плохо.
	Сорян =(
*/
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
out vec4 Color;                                                                     \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
    Color = vec4(clamp(Position, 0.0, 1.0), 1.0);                                   \n\
}"; /*Вершинный шейдер*/

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec4 Color;                                                                      \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = Color;                                                              \n\
}"; /*Пиксельный шейдер*/


static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0;

	Scale += 0.01;

    Pipeline p;
    p.Scale(sinf(Scale * 0.1), sinf(Scale * 0.1), sinf(Scale * 0.1));
    p.WorldPos(sinf(Scale), 0.0, 0.0);
    p.Rotate(sinf(Scale) * 90.0, sinf(Scale) * 90.0, sinf(Scale) * 90.0);

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans()); // Плавно и красиво изменяем отображение фигуры на экране

	glEnableVertexAttribArray(0); // Разрешаем использование атрибута вершины для доступа к нему через конвеер. Это необходимо для шейдеров
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Привязываем указатель на вершины для отрисовки кадра
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Этот вызов говорит конвейеру как воспринимать данные внутри буфера.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // Привязываем указатель на индексы для отрисовки кадра

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0); /*
										Это признак хорошего тона отключать каждый атрибут вершины, как только отпадает необходимость в нем. 
										Оставить его включенным, пока шейдер не используется - лучший способ получить проблем.
								   */

	glutSwapBuffers(); /*
							После рисования нужно просить OpenGL сменить экранные буфера при помощи glutSwapBuffers (), ведь у нас включена двойная буферизация. 
							Все рисуется на скрытом от пользователя буфере и затем происходит смена буферов. 
							Делается это для получения плавной анимации и для того, чтобы не было эффекта мерцания экрана.
					   */
}


static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB); // Указываем функцию рендера в качестве "ленивой"
}

static void CreateVertexBuffer()
{
	Vector3f Vertices[8];
	Vertices[0] = Vector3f(0.5, -0.5, -0.5);
	Vertices[1] = Vector3f(-0.5, -0.5, -0.5);
	Vertices[2] = Vector3f(-0.5, 0.5, -0.5);
	Vertices[3] = Vector3f(0.5, 0.5, -0.5);
	Vertices[4] = Vector3f(0.5, -0.5, 0.5);
	Vertices[5] = Vector3f(-0.5, -0.5, 0.5);
	Vertices[6] = Vector3f(-0.5, 0.5, 0.5);
	Vertices[7] = Vector3f(0.5, 0.5, 0.5);

	glGenBuffers(1, &VBO); // Создаем буфер в общем типе. Для указания задачи используется следующая функция.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Привязываем указатель для наполнения данными
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // После связывания нашего объекта, мы наполняем его данными.
}

static void CreateIndexBuffer()
{
	unsigned int Indices[] = {  0, 1, 2,
								2, 3, 0,
								0, 4, 3,
								3, 7, 4,
								4, 0, 1,
								1, 4, 5,
								5, 1, 2,
								2, 5, 6,
								6, 2, 3,
								3, 6, 7,
								7, 4, 5,
								5, 6, 7 };

	glGenBuffers(1, &IBO); // Здесь и ниже: всё аналогично CreateVertexBuffer()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* shdrTxt[1];
	shdrTxt[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, shdrTxt, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj); // Присоединяем скомпилированный объект шейдера к объекту программы
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram(); /*
													Мы начинаем процесс разработки шейдеров через создание программного объекта. 
													Позже мы слинкуем все шейдеры в этот объект.
											  */

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER); // Создание вершинного шейдера
	AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER); // Создание фрагментного (пиксельного) шейдера

	GLint success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram); /*
										Для использования отлинкованной программы шейдеров мы назначаем её для конвейера используя вызов выше. 
										Эта программа сохранит эффект для всех вызовов отрисовки, пока вы не замените её другой 
										или не запретите её использование напрямую функцией glUseProgram с параметром NULL. 
										Если вы создадите шейдерную программу, содержащую только 1 тип шейдеров, 
										тогда другие этапы будут использовать свою функциональность по-умолчанию.
								 */

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld"); // Запрашиваем позицию uniform-переменной в программном объекте
	assert(gWorldLocation != 0xFFFFFFFF); /*
												Очень важна проверка на ошибки (как мы и сделали тут), иначе обновления переменной не попадут в шейдер. 
												Есть 2 основные причины ошибки у этой функции. 
												Вы написали с ошибкой имя переменной или она была убрана компилятором с целью оптимизации. 
												Если компилятор не обнаружит использования переменной, он без раздумий выбросит её. 
												В этом случае glGetUniformLocation не даст результата.
										  */
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv); // Инициализируем GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*
														Так настраиваются некоторые опции GLUT. 
														GLUT_DOUBLE включает двойную буферизацию (отрисовка будет происходить в фоновый буфер, в то время как другой буфер отображается) 
														и буфер цвета, непрерывно используемого для конечной цели рендеринга (т.е. на экране).
												  */
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial XX"); // Здесь задается заголовок окна. В оригинальных уроках у каждого туториала есть индивидуальный двузначный номер. Здесь он заменен на ХХ.

	InitializeGlutCallbacks(); // Большая часть взаимодействий с системой происходит через функции обратного вызова

	/* Теперь мы инициализируем GLEW и проверяем на ошибки. GLUT необходимо инициализировать раньше. */
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(err));
		return 1;
	}

	glClearColor(0.0, 0.0, 0.0, 0.0); // Установка "чистого" цвета фона

	CreateVertexBuffer();
	CreateIndexBuffer();

	CompileShaders(); // Компилируем шейдеры

	glutMainLoop(); /*
						Этот вызов передаёт контроль GLUT’у, который теперь начнёт свой собственный цикл. 
						В этом цикле он ждёт событий от оконной системы и передаёт их через функции обратного вызова, которые мы задали ранее.
					*/

	return 0;
}
