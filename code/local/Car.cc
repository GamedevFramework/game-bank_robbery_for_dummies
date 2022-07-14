/*
 * Bank Robbery for Dummies
 * Copyright (C) 2015 JARS
 * Copyright (C) 2017 Julien Bernard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Car.h"

#include <gf/Math.h>
#include <gf/Matrix.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Transform.h>
#include <gf/VectorOps.h>

#include "Messages.h"

#include <iostream>

namespace brfd {

  static constexpr float CarWidth = 128;
  static constexpr float CarHeight = 64;
  static constexpr float CarHalfWidth = CarWidth / 2;
  static constexpr float CarHalfHeight = CarHeight / 2;
  static constexpr float X = 0.8f;

  static constexpr float TilesetWidth = 512;
  static constexpr float TilesetHeight = 192;

  static const gf::Vector2f Geometry[] {
    { -     CarHalfWidth,   X * CarHalfHeight },
    { - X * CarHalfWidth,       CarHalfHeight },
    {   X * CarHalfWidth,       CarHalfHeight },
    {       CarHalfWidth,   X * CarHalfHeight },
    {       CarHalfWidth, - X * CarHalfHeight },
    {   X * CarHalfWidth, -     CarHalfHeight },
    { - X * CarHalfWidth, -     CarHalfHeight },
    { -     CarHalfWidth, - X * CarHalfHeight },
  };

  static constexpr float CarMass = 1000.0f;

  gfcp::PolygonShape computeCarShape(gfcp::Body body) {
    gfcp::PolygonShape shape(body, Geometry, gf::identityTransform(), 1.0f);
    shape.setElasticity(0.0f);
    shape.setFriction(0.1f);
    return shape;
  }

  HeroCar::HeroCar(gf::MessageManager& messages, gf::ResourceManager& resources, gfcp::Space& physics)
  : gf::Entity(1)
  , m_messages(messages)
  , m_texture(resources.getTexture("cars.png"))
  , m_move(gf::LinearMove::None)
  , m_turn(gf::AngularMove::None)
  , m_velocity(0)
  , m_angle(0)
  , m_body(CarMass, gfcp::computeMomentForPoly(CarMass, Geometry, gf::vec(0.0f, 0.0f), 0.0f))
  {
    m_texture.setSmooth();
    physics.addBody(m_body);
    auto shape = computeCarShape(m_body);
    physics.addShape(shape);
  }

  void HeroCar::startAt(gf::Vector2f position, float angle) {
    m_body.setPosition(position);
    m_body.setAngle(angle);
  }

  static constexpr float LinearAcceleration = 800.0f;
  static constexpr float AngularSpeed = 2.0f;
  static constexpr float VelocityMax = 1500.0f;
  static constexpr float VelocityMin = -500.0f;

  void HeroCar::update(gf::Time time) {
    float dt = time.asSeconds();
    gf::Vector2f position = m_body.getPosition();

    HeroPosition msg;
    msg.position = position;
    m_messages.sendMessage(&msg);

    float absoluteVelocity = gf::euclideanLength(m_body.getVelocity());
    m_velocity =  m_velocity > 0 ? absoluteVelocity : -absoluteVelocity;

    m_velocity += gf::linearFactor(m_move) * LinearAcceleration * dt;
    m_velocity = gf::clamp(m_velocity, VelocityMin, VelocityMax);

    float factor = 0.006f * std::abs(m_velocity) * std::exp(-0.002f * std::abs(m_velocity));
    m_angle = m_body.getAngle();

    m_angle += gf::sign(m_velocity) * gf::angularFactor(m_turn) * AngularSpeed * factor * dt;

    m_body.setVelocity(gf::unit(m_angle) * m_velocity);
    m_body.setAngle(m_angle);
    m_body.setAngularVelocity(0.0f);
  }

  static constexpr gf::Vector2f TileScale(CarWidth / TilesetWidth, CarHeight / TilesetHeight);

  void HeroCar::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Sprite sprite(m_texture, gf::RectF::fromPositionSize(TileScale * gf::Vector2u(1, 1), TileScale));
    sprite.setAnchor(gf::Anchor::Center);
    sprite.setRotation(m_body.getAngle());
    sprite.setPosition(m_body.getPosition());

    target.draw(sprite, states);

//     m_body.render(target);
  }


  StaticCar::StaticCar(int number, gf::Vector2f position, float angle)
  : m_body(CarMass, gfcp::computeMomentForPoly(CarMass, Geometry, gf::vec(0.0f, 0.0f), 0.0f))
  {
    m_textureRect = gf::RectF::fromPositionSize({ (number % 4) * CarWidth / TilesetWidth, (number / 4) * CarHeight / TilesetHeight }, { CarWidth / TilesetWidth, CarHeight / TilesetHeight });
    m_body.setPosition(position);
    m_body.setAngle(angle);
  }

  void StaticCar::update([[maybe_unused]] gf::Time time) {
    float angle = m_body.getAngle();
    gf::Vector2f velocity = m_body.getVelocity();
    gf::Vector2f normal = gf::unit(angle + gf::Pi2);
    m_body.setVelocity(velocity - gf::dot(velocity, normal) * normal);
    m_body.setAngularVelocity(0.0f);
  }

}
