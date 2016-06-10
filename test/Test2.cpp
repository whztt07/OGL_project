#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\src\Headers\Logic_Nim.h"
#include "..\src\Headers\Bunches.h"
#include "..\src\Headers\Save.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Save_and_Load_Test
{		
	TEST_CLASS(Savedata_Test)
	{
	public:
		TEST_METHOD(Test_Savedata_slot)
		{
			Logic_Nim nim;
			nim.default_settings();
			nim.game_mode = Logic_Nim::bot_vs_player;
			nim.difficulty = Logic_Nim::hard;
			nim.who_do_move = Logic_Nim::bot;
			Bunches bun;
			bun.form = Bunches::cir;
			char *str_music = "Test";

			for (int i = 0; i < 5; i++) {
				bool done = Savedata(i, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music);
				Assert::IsTrue(done);
			}
			for (int i = -100; i < 0; i++) {
				bool done = Savedata(i, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music);
				Assert::IsFalse(done);
			}
			for (int i = 5; i < 100; i++) {
				bool done = Savedata(i, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music);
				Assert::IsFalse(done);
			}
		}
		TEST_METHOD(Test_Savedata_bunches)
		{
			Logic_Nim nim;
			nim.default_settings();
			nim.game_mode = Logic_Nim::bot_vs_player;
			nim.difficulty = Logic_Nim::hard;
			nim.who_do_move = Logic_Nim::bot;
			Bunches bun;
			bun.form = Bunches::cir;
			char *str_music = "Test";

			//Все, что справедливо для одной кучки, то справедливо и для остальных
			for (int i = 0; i <= Logic_Nim::max_num_of_stones; i++) {
				nim.array_bunches[0] = i;
				bool done = Savedata(0, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music);
				Assert::IsTrue(done);
			}
			for (int i = -100; i < 0; i++) {
				nim.array_bunches[0] = i;
				bool done = Savedata(0, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music);
				Assert::IsFalse(done);
			}
			for (int i = Logic_Nim::max_num_of_stones + 1; i < Logic_Nim::max_num_of_stones + 100; i++) {
				nim.array_bunches[0] = i;
				bool done = Savedata(0, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music);
				Assert::IsFalse(done);
			}
		}
	};
	
	//После тестов сохранений создадуться 5 файлов сохранений с характеристиками: %ДАТА_И_ВРЕМЯ%  0 0 1 1 %КОЛ_КАМ_В_ПЕР_КУЧ% 8 10 Test %КОЛ_КАМ_В_ПЕР_КУЧ% равно 10 для первого сохранения и чему-то случайному для всех остальных.
	TEST_CLASS(Loaddata_Test)
	{
	public:
		TEST_METHOD(Test_Loaddata_slot)
		{
			Logic_Nim nim;
			nim.default_settings();
			nim.game_mode = Logic_Nim::bot_vs_player;
			nim.difficulty = Logic_Nim::hard;
			nim.who_do_move = Logic_Nim::bot;
			Bunches bun;
			bun.form = Bunches::cir;
			char str_music[80];

			for (int i = 0; i < 5; i++) {
				bool done = Loaddata(i, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music);
				Assert::IsTrue(done);
			}
			for (int i = -100; i < 0; i++) {
				bool done = Loaddata(i, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music);
				Assert::IsFalse(done);
			}
			for (int i = 5; i < 100; i++) {
				bool done = Loaddata(i, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music);
				Assert::IsFalse(done);
			}
		}
		TEST_METHOD(Test_Loaddata_bunches)
		{
			Logic_Nim nim;
			nim.default_settings();
			nim.game_mode = Logic_Nim::bot_vs_player;
			nim.difficulty = Logic_Nim::hard;
			nim.who_do_move = Logic_Nim::bot;
			Bunches bun;
			bun.form = Bunches::cir;
			char str_music[80];

			for (int i = -100; i <= Logic_Nim::max_num_of_stones+100; i++) {
				nim.array_bunches[0] = i;
				bool done = Loaddata(0, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music);
				Assert::AreEqual(nim.array_bunches[0], 10);
			}
		}
	};
}