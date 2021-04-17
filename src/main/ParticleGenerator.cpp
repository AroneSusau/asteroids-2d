#include "../headers/classes/ParticleGenerator.h"

void ParticleGenerator::create_particle() {
  for (int i = 0; i < density; ++i) {
    Particle_t particle;
  
    particle.x = util.random(x_min, x_max);
    particle.y = util.random(y_min, y_max);
    particle.rotation = util.random(spread_min, spread_max) - spread_max/2;
    particle.dx = util.random(dx_min, dx_max) * cosf(util.deg_to_rad(particle.rotation));
    particle.dy = util.random(dy_min, dy_max) * sinf(util.deg_to_rad(particle.rotation));
    particle.lifetime = particle_lifetime;
    particle.remaining = particle_lifetime;
    particle.size = size;
    particle.colour[0] = start_colour[0];
    particle.colour[1] = start_colour[1];
    particle.colour[2] = start_colour[2];

    particles.push_back(particle);
  }
}

void ParticleGenerator::movement(float dt) {
  for (size_t i = 0; i < particles.size(); ++i) {
  
    Particle_t &particle = particles.at(i);
    particle.remaining -= dt;

    update_colour(particle);

    if (particle.remaining <= 0) {
      particles.erase(particles.begin() + i);
    } else {
      particle.x -= particle.dx;
      particle.y -= particle.dy;
      particle.size *= 0.6; 
    }
  }
}

void ParticleGenerator::update_colour(Particle_t &particle) {
  float ratio = particle.remaining / particle.lifetime;
  
  particle.colour[0] = gradient_colour[0] - (gradient_colour[0] - start_colour[0]) * ratio;
  particle.colour[1] = gradient_colour[1] - (gradient_colour[1] - start_colour[1]) * ratio;
  particle.colour[2] = gradient_colour[2] - (gradient_colour[2] - start_colour[2]) * ratio;
}

void ParticleGenerator::update_lifetime(float dt) {

  if (duration_type == discrete && active) {
    remaining -= dt;

    if (remaining <= 0) {
      active = false;
    }
  }
}

void ParticleGenerator::spawn(float dt) {
  if (dt <= rate) {
    create_particle();
  }
}

void ParticleGenerator::render() {
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  glTranslatef(x, y, 0.0f);
  glRotatef(rotation, 0, 0, 1); 
  glTranslatef(-x, -y, 0.0f);

  for (size_t i = 0; i < particles.size(); ++i) {
    
    Particle_t &particle = particles.at(i);

    glPushMatrix();
    glTranslatef(particle.x + offset_x, particle.y + offset_y, 0.0f);
    
    glColor3f(particle.colour[0], particle.colour[1], particle.colour[2]);

    glPointSize(particle.size);
    glBegin(GL_POINTS);
      glVertex2d(0, 0);
    glEnd();
    glPopMatrix();
  }

  glPopMatrix();
  glPointSize(1);
}