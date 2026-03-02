#include "club.hpp"
#include "events.hpp"
#include <gtest/gtest.h>

TEST(ClubTest, ConstructorInitializesCorrectly) {
  club_config config{3, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  EXPECT_EQ(club.open_time().hours(), 9);
  EXPECT_EQ(club.close_time().hours(), 19);
  EXPECT_EQ(club.get_stats().size(), 3u);
}

TEST(ClubTest, ClientArrivedBeforeOpenTime) {
  club_config config{3, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto event =
      std::make_unique<ClientArrivedEvent>(TimePoint(8, 30), "client1");
  auto output = club.process_event(std::move(event));

  ASSERT_EQ(output.size(), 2u);
  EXPECT_EQ(output[0]->id(), 1);
  EXPECT_EQ(output[1]->id(), 13);
  EXPECT_EQ(output[1]->to_string(), "08:30 13 NotOpenYet");
}

TEST(ClubTest, ClientArrivedSuccessfully) {
  club_config config{3, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto event =
      std::make_unique<ClientArrivedEvent>(TimePoint(9, 30), "client1");
  auto output = club.process_event(std::move(event));

  ASSERT_EQ(output.size(), 1u);
  EXPECT_EQ(output[0]->id(), 1);
}

TEST(ClubTest, ClientArrivedTwice) {
  club_config config{3, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto event1 =
      std::make_unique<ClientArrivedEvent>(TimePoint(9, 30), "client1");
  club.process_event(std::move(event1));

  auto event2 =
      std::make_unique<ClientArrivedEvent>(TimePoint(10, 0), "client1");
  auto output = club.process_event(std::move(event2));

  ASSERT_EQ(output.size(), 2u);
  EXPECT_EQ(output[1]->id(), 13);
  EXPECT_EQ(output[1]->to_string(), "10:00 13 YouShallNotPass");
}

TEST(ClubTest, ClientSatSuccessfully) {
  club_config config{3, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto event1 =
      std::make_unique<ClientArrivedEvent>(TimePoint(9, 30), "client1");
  club.process_event(std::move(event1));

  auto event2 =
      std::make_unique<ClientSatEvent>(TimePoint(9, 35), "client1", 2);
  auto output = club.process_event(std::move(event2));

  ASSERT_EQ(output.size(), 1u);
  EXPECT_EQ(output[0]->id(), 2);
}

TEST(ClubTest, ClientSatUnknown) {
  club_config config{3, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto event = std::make_unique<ClientSatEvent>(TimePoint(9, 35), "client1", 2);
  auto output = club.process_event(std::move(event));

  ASSERT_EQ(output.size(), 2u);
  EXPECT_EQ(output[1]->id(), 13);
  EXPECT_EQ(output[1]->to_string(), "09:35 13 ClientUnknown");
}

TEST(ClubTest, ClientSatPlaceIsBusy) {
  club_config config{3, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto event1 =
      std::make_unique<ClientArrivedEvent>(TimePoint(9, 30), "client1");
  club.process_event(std::move(event1));
  auto event2 =
      std::make_unique<ClientSatEvent>(TimePoint(9, 35), "client1", 2);
  club.process_event(std::move(event2));

  auto event3 =
      std::make_unique<ClientArrivedEvent>(TimePoint(9, 40), "client2");
  club.process_event(std::move(event3));
  auto event4 =
      std::make_unique<ClientSatEvent>(TimePoint(9, 45), "client2", 2);
  auto output = club.process_event(std::move(event4));

  ASSERT_EQ(output.size(), 2u);
  EXPECT_EQ(output[1]->id(), 13);
  EXPECT_EQ(output[1]->to_string(), "09:45 13 PlaceIsBusy");
}

TEST(ClubTest, ClientWaitingWithFreeTables) {
  club_config config{3, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto event1 =
      std::make_unique<ClientArrivedEvent>(TimePoint(9, 30), "client1");
  club.process_event(std::move(event1));

  auto event2 =
      std::make_unique<ClientWaitingEvent>(TimePoint(9, 35), "client1");
  auto output = club.process_event(std::move(event2));

  ASSERT_EQ(output.size(), 2u);
  EXPECT_EQ(output[1]->id(), 13);
  EXPECT_EQ(output[1]->to_string(), "09:35 13 ICanWaitNoLonger!");
}

TEST(ClubTest, ClientWaitingQueueFull) {
  club_config config{2, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto e1 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 0), "client1");
  club.process_event(std::move(e1));
  auto e2 = std::make_unique<ClientSatEvent>(TimePoint(9, 1), "client1", 1);
  club.process_event(std::move(e2));

  auto e3 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 2), "client2");
  club.process_event(std::move(e3));
  auto e4 = std::make_unique<ClientSatEvent>(TimePoint(9, 3), "client2", 2);
  club.process_event(std::move(e4));

  auto e5 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 4), "client3");
  club.process_event(std::move(e5));
  auto e6 = std::make_unique<ClientWaitingEvent>(TimePoint(9, 5), "client3");
  club.process_event(std::move(e6));

  auto e7 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 6), "client4");
  club.process_event(std::move(e7));
  auto e8 = std::make_unique<ClientWaitingEvent>(TimePoint(9, 7), "client4");
  club.process_event(std::move(e8));

  auto e9 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 8), "client5");
  club.process_event(std::move(e9));
  auto e10 = std::make_unique<ClientWaitingEvent>(TimePoint(9, 9), "client5");
  auto output = club.process_event(std::move(e10));

  ASSERT_EQ(output.size(), 2u);
  EXPECT_EQ(output[1]->id(), 11);
  EXPECT_EQ(output[1]->to_string(), "09:09 11 client5");
}

TEST(ClubTest, ClientLeftSuccessfully) {
  club_config config{3, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto event1 =
      std::make_unique<ClientArrivedEvent>(TimePoint(9, 30), "client1");
  club.process_event(std::move(event1));
  auto event2 =
      std::make_unique<ClientSatEvent>(TimePoint(9, 35), "client1", 2);
  club.process_event(std::move(event2));

  auto event3 = std::make_unique<ClientLeftEvent>(TimePoint(10, 30), "client1");
  auto output = club.process_event(std::move(event3));

  ASSERT_EQ(output.size(), 1u);
  EXPECT_EQ(output[0]->id(), 4);
}

TEST(ClubTest, ClientLeftFreesTableForWaiting) {
  club_config config{1, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto e1 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 0), "client1");
  club.process_event(std::move(e1));
  auto e2 = std::make_unique<ClientSatEvent>(TimePoint(9, 1), "client1", 1);
  club.process_event(std::move(e2));

  auto e3 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 2), "client2");
  club.process_event(std::move(e3));
  auto e4 = std::make_unique<ClientWaitingEvent>(TimePoint(9, 3), "client2");
  club.process_event(std::move(e4));

  auto e5 = std::make_unique<ClientLeftEvent>(TimePoint(9, 10), "client1");
  auto output = club.process_event(std::move(e5));

  ASSERT_EQ(output.size(), 2u);
  EXPECT_EQ(output[0]->id(), 4);
  EXPECT_EQ(output[1]->id(), 12);
  EXPECT_EQ(output[1]->to_string(), "09:10 12 client2 1");
}

TEST(ClubTest, CloseClubKicksAllClients) {
  club_config config{3, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto e1 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 0), "client2");
  club.process_event(std::move(e1));
  auto e2 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 1), "client1");
  club.process_event(std::move(e2));
  auto e3 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 2), "client3");
  club.process_event(std::move(e3));

  auto output = club.close_club();

  ASSERT_EQ(output.size(), 3u);
  EXPECT_EQ(output[0]->to_string(), "19:00 11 client1");
  EXPECT_EQ(output[1]->to_string(), "19:00 11 client2");
  EXPECT_EQ(output[2]->to_string(), "19:00 11 client3");
}

TEST(ClubTest, CalculateTableRevenue) {
  club_config config{1, TimePoint(9, 0), TimePoint(19, 0), 10};
  Club club(config);

  auto e1 = std::make_unique<ClientArrivedEvent>(TimePoint(9, 0), "client1");
  club.process_event(std::move(e1));
  auto e2 = std::make_unique<ClientSatEvent>(TimePoint(9, 0), "client1", 1);
  club.process_event(std::move(e2));

  auto e3 = std::make_unique<ClientLeftEvent>(TimePoint(11, 30), "client1");
  club.process_event(std::move(e3));

  const auto &stats = club.get_stats();
  EXPECT_EQ(stats[0].total_minutes, 150);
  EXPECT_EQ(stats[0].revenue, 30);
}
