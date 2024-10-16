#include "testscene.h"
#include <ext/matrix_clip_space.hpp>

void testscene::start()
{
    const std::string fname = "blank.png";
    const std::string objname = "dragon.txt";
    MeshComponent m = MeshBuilder::loadFromObj(&objname, &fname);


    
    
    
    
    TransformComponent t;
    t.position = { 0,0,0 };
    t.scale = { 2,2,2 };

    TransformComponent t1;
    t1.position = { 0, 10.0, 10 };
    t1.scale = { 1,1,1 };

    TransformComponent t2;
    t2.position = { 0, 5,5 };
    t2.scale = { 1,1,1 };

    CameraComponent camera;
    //camera.position = glm::vec3(0, 10.0, 10);
    camera.camFront = glm::vec3(0.0, 0.0, -1.0);
    camera.camUp = glm::vec3(0.0, 1.0, 0.0);
    camera.projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    PointLightComponent p;
    p.attenuation = { 1,0.14,.07 };
    p.color = { 0.894, 0.721, 0.043 };

    RigidBodyComponent rb;
    rb.mass = 0.01;


    SphereColliderComponent s;
    s.radius = 2;

    SphereColliderComponent s1;
    s1.radius = 7;

    AABBColliderComponent a;
    AABBColliderComponent a1;
    a.dimensions = { 3,3,3 };
    a1.dimensions = { 15,15,15 };

    SATColliderComponent sat;
    std::vector<glm::vec3> verts;
    for (Vertex v : m.verts) {
        verts.push_back(v.position);
    }
    sat.verts = &(verts);
         
      //glm::vec3(-20.0f, -20.0f, -20.0f), // Vertex 0: Bottom-left-back
    //    glm::vec3(20.0f, -20.0f, -20.0f), // Vertex 1: Bottom-right-back
    //    glm::vec3(20.0f, 20.0f, -20.0f), // Vertex 2: Top-right-back
    //    glm::vec3(-20.0f, 20.0f, -20.0f), // Vertex 3: Top-left-back
    //    glm::vec3(-20.0f, -20.0f, 20.0f), // Vertex 4: Bottom-left-front
    //    glm::vec3(20.0f, -20.0f, 20.0f), // Vertex 5: Bottom-right-front
    //    glm::vec3(20.0f, 20.0f, 20.0f), // Vertex 6: Top-right-front
    //    glm::vec3(-20.0f, 20.0f, 20.0f)  // Vertex 7: Top-left-front
    std::shared_ptr<MeshComponent> mp = std::make_shared<MeshComponent>(m);
    //{ {0, 0, 0}, { 0,0,10 }, { 0,10,10 }, { 0,10,0 }, { 0,0,0 }, { 10,0,0 }, { 10,0,10 }, { 10,10,10 }, { 10,10,0 }, { 10,0,0 } });
    
    DirectionalLightComponent d;
    d.color = { 0,0,0 };
    d.direction = { 0.5,0.5,0 };
    ColliderComponent c;
    c.type = Sphere;
    manager.registerComponent<MeshComponent, TransformComponent, CameraComponent,DirectionalLightComponent,PointLightComponent,RigidBodyComponent,ColliderComponent>(m,t,camera,d,p,rb,c);
    manager.addComponent<MeshComponent>(1, mp);
    manager.addComponent<TransformComponent>(1, std::make_shared<TransformComponent>(t));
    manager.addComponent<RigidBodyComponent>(1, std::make_shared<RigidBodyComponent>(rb));
    manager.addComponent<ColliderComponent>(1, std::make_shared<SATColliderComponent>(sat));

    manager.addComponent<CameraComponent>(0, std::make_shared<CameraComponent>(camera));
    manager.addComponent<ColliderComponent>(0, std::make_shared<SphereColliderComponent>(s));
    manager.addComponent<TransformComponent>(0, std::make_shared<TransformComponent>(t1));

    manager.addComponent<DirectionalLightComponent>(2, std::make_shared<DirectionalLightComponent>(d));
    /*for (int i = 4; i < 10; i++) {
        manager.addComponent<MeshComponent>(i, mp);
        TransformComponent lt;
        lt.position = { 10 * i,5,0 };
        lt.scale = { 1,1,1 };
        manager.addComponent<TransformComponent>(i, std::make_shared<TransformComponent>(lt));
    }*/
    manager.addComponent<PointLightComponent>(4, std::make_shared<PointLightComponent>(p));
    manager.addComponent<TransformComponent>(4, std::make_shared<TransformComponent>(t2));


    
    behaviors.push_back(&renderer);
    behaviors.push_back(&controller);
    behaviors.push_back(&physics);
    for (Behavior* behavior : behaviors) {
        behavior->init();
    }
   
}

void testscene::update()
{
    for (Behavior* behavior : behaviors) {
        
        behavior->update(&manager);
        
    }
    
    
}

void testscene::close()
{
}
