#include "Service.h"

Service::Service(Session &session) : session(session) {}

Stream<Measurement> *Service::measurements(string sensor_id,
                                           Maybe<MeasurementType> type,
                                           Maybe<Timestamp> timestamp) {}

Result<double, const char *> Service::air_quality_area(double x, double y,
                                                       double rad,
                                                       Maybe<Timestamp> start,
                                                       Maybe<Timestamp> end) {}

Stream<Sensor> *Service::similar_sensors(string sensor_id, int n,
                                         Maybe<Timestamp> start,
                                         Maybe<Timestamp> end) {}

Result<double, const char *> Service::cleaner_efficiency(string cleaner_id) {}

Result<double, const char *>
Service::provider_cleaners_efficiency(string provider) {}

void Service::flag_owner(string owner_id, OwnerFlag flag) {
  auto filter = [&](const Owner &owner) -> bool {
    return owner.get_id() == owner_id;
  };
  auto owner_stream = session.owners_db.filter_and_stream(filter);
  if (!owner_stream->receive().is_absent) {
    session.owners_flags.insert(make_pair(owner_id, flag));
  }
}

Maybe<const char *> Service::authenticate(string username, string password) {}

Maybe<User> Service::authenticated_user() { return session.authed_user; }
