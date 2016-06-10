#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\src\Headers\Logic_Nim.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Logic_Nim_Test
{		
	TEST_CLASS(Logic_Nim_Test)
	{
	public:
		
		TEST_METHOD(Test_default_settings)
		{
			Logic_Nim nim;
			nim.default_settings();
		
			Assert::AreEqual(nim.chosen_bunch, -1);
			Assert::AreEqual(nim.chosen_stones, -1);
			Assert::IsTrue(nim.array_bunches[0] <= nim.array_bunches[1] &&
				nim.array_bunches[1] <= nim.array_bunches[2]);
			for (int i = 0; i < nim.num_of_bunches; i++)
				Assert::IsTrue(nim.array_bunches[i] <= nim.max_num_of_stones
					&& nim.array_bunches[i] >= nim.get_min_num_of_stones());
		}

		TEST_METHOD(Test_nim_sum)
		{
			Logic_Nim nim;
			nim.set_amount_stones_in_bunches(3, 5, 7);
			Assert::IsTrue(nim.nim_sum() == 1);
			nim.set_amount_stones_in_bunches(3, 5, 6);
			Assert::IsTrue(nim.nim_sum() == 0);
		}

		TEST_METHOD(Test_bot_choosing)
		{
			Logic_Nim nim;

			nim.set_amount_stones_in_bunches(3, 5, 7);
			nim.bot_choosing();
			Assert::AreEqual(nim.chosen_bunch, 2);
			Assert::AreEqual(nim.chosen_stones, 6);

			nim.set_amount_stones_in_bunches(3, 5, 6);
			nim.bot_choosing();
			Assert::IsTrue(nim.chosen_bunch >= 0 &&
				nim.chosen_bunch < nim.num_of_bunches);
			Assert::IsTrue(nim.chosen_stones >= 0 &&
				nim.chosen_stones < nim.array_bunches[nim.chosen_bunch]);
		}

		TEST_METHOD(test_hard_choose_bunch_bot)
		{
			Logic_Nim nim;
			nim.set_amount_stones_in_bunches(3, 5, 7);
			int a = nim.hard_choose_bunch_bot();
			Assert::AreEqual(a, 2);
		}

		TEST_METHOD(test_hard_choose_stones_bot)
		{
			Logic_Nim nim;
			nim.set_amount_stones_in_bunches(3, 5, 7);
			nim.chosen_bunch = 2;
			Assert::AreEqual(nim.hard_choose_stones_bot(), 6);
		}

		TEST_METHOD(test_swap_priority)
		{
			Logic_Nim nim;
			nim.game_mode = Logic_Nim::player1_vs_player2;
			nim.who_do_move = Logic_Nim::player1;
			nim.swap_priority();
			Assert::IsTrue(nim.who_do_move == Logic_Nim::player2);
			nim.who_do_move = Logic_Nim::player2;
			nim.swap_priority();
			Assert::IsTrue(nim.who_do_move == Logic_Nim::player1);
			
			nim.game_mode = Logic_Nim::bot_vs_player;
			nim.who_do_move = Logic_Nim::player;
			nim.swap_priority();
			Assert::IsTrue(nim.who_do_move == Logic_Nim::bot);
			nim.who_do_move = Logic_Nim::bot;
			nim.swap_priority();
			Assert::IsTrue(nim.who_do_move == Logic_Nim::player);
		}

		TEST_METHOD(test_do_turn)
		{
			Logic_Nim nim;
			nim.set_amount_stones_in_bunches(3, 5, 7);
			nim.chosen_bunch = 1;
			nim.chosen_stones = 3;
			nim.do_turn();
			Assert::AreEqual(nim.array_bunches[1], 3);

			nim.set_amount_stones_in_bunches(3, 5, 7);
			nim.chosen_bunch = 2;
			nim.chosen_stones = 4;
			nim.do_turn();
			Assert::AreEqual(nim.array_bunches[2], 4);

		}

		TEST_METHOD(test_end_of_game)
		{
			Logic_Nim nim;
			nim.set_amount_stones_in_bunches(2, 6, 6);
			Assert::IsFalse(nim.end_of_game());

			nim.set_amount_stones_in_bunches(0, 8, 10);
			Assert::IsFalse(nim.end_of_game());

			nim.set_amount_stones_in_bunches(0, 0, 0);
			Assert::IsTrue(nim.end_of_game());
		}
	};
}