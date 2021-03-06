#include "Graphics/Graphics.h"

#include <Core.h>
#include <gtest/gtest.h>

#include <memory>

#include "Graphics/Camera.h"
#include "Graphics/Sprite.h"

namespace asd = altseed;

TEST(Graphics, Initialize) {

#if defined(__APPLE__) || defined(__linux__)
    return;
#endif

    EXPECT_TRUE(asd::Core::Initialize(u"test", 1280, 720, asd::CoreOption()));

    int count = 0;

    auto instance = altseed::Graphics::GetInstance();
    EXPECT_TRUE(instance != nullptr);
    auto renderer = instance->CreateRenderer();
    EXPECT_TRUE(renderer != nullptr);

    auto t1 = instance->CreateDameyTexture(0);
    EXPECT_TRUE(t1 != nullptr);

    auto t2 = instance->CreateDameyTexture(255);
    EXPECT_TRUE(t2 != nullptr);

    auto shader = instance->CreateShader(renderer->HlslPSCode);
	EXPECT_TRUE(shader != nullptr);
    auto material = std::make_shared<altseed::Material>();
    EXPECT_TRUE(material != nullptr);

    material->SetShader(shader);

    {
        int c = 0;
        for (int x = 0; x < 5; x++) {
            for (int y = 0; y < 5; y++) {
                auto sprite = std::make_shared<altseed::Sprite>();
                sprite->SetMaterial(material);
                sprite->SetPosition(asd::Vector2DF(x * 120, y * 120));
                sprite->SetSize(asd::Vector2DF(80, 80));
                sprite->SetTexture(c++ % 2 == 0 ? t1 : t2);
                renderer->Sprites.push_back(sprite);
            }
        }
    }

    while (count++ < 100 && instance->DoEvents()) EXPECT_TRUE(instance->Update());

    altseed::Graphics::Terminate();
}

TEST(Graphics, Texture) {

#if defined(__APPLE__) || defined(__linux__)
    return;
#endif

    EXPECT_TRUE(asd::Core::Initialize(u"test", 1280, 720, asd::CoreOption()));

    int count = 0;

    auto instance = altseed::Graphics::GetInstance();
    auto renderer = instance->CreateRenderer();

    auto t1 = asd::Texture2D::Load(u"TestData/IO/AltseedPink.png");
    auto t2 = asd::Texture2D::Load(u"TestData/IO/AltseedPink.jpg");

    EXPECT_TRUE(t1 != nullptr);
    EXPECT_TRUE(t2 != nullptr);

    auto shader = instance->CreateShader(renderer->HlslPSCode);
    auto material = std::make_shared<altseed::Material>();
    material->SetShader(shader);

    {
        int c = 0;
        for (int x = 0; x < 5; x++) {
            for (int y = 0; y < 5; y++) {
                auto sprite = std::make_shared<altseed::Sprite>();
                sprite->SetMaterial(material);
                sprite->SetPosition(asd::Vector2DF(x * 120, y * 120));
                sprite->SetSize(asd::Vector2DF(80, 80));
                sprite->SetTexture(c++ % 2 == 0 ? t1->GetNativeTexture() : t2->GetNativeTexture());
                renderer->Sprites.push_back(sprite);
            }
        }
    }

    while (count++ < 100 && instance->DoEvents()) EXPECT_TRUE(instance->Update());

    altseed::Graphics::Terminate();
}

TEST(Graphics, Camera) {

#if defined(__APPLE__) || defined(__linux__)
    return;
#endif

    EXPECT_TRUE(asd::Core::Initialize(u"test", 1280, 720, asd::CoreOption()));

    int count = 0;

    auto instance = altseed::Graphics::GetInstance();
    auto renderer = instance->CreateRenderer();

    auto t1 = instance->CreateDameyTexture(0);
    auto t2 = instance->CreateDameyTexture(255);
    auto cam = std::make_shared<asd::Camera>();
    auto rt = std::make_shared<asd::RenderTexture>(asd::Vector2DI(256, 256));
    cam->SetTarget(rt);
    renderer->Cameras.push_back(cam);

    EXPECT_TRUE(t1 != nullptr);
    EXPECT_TRUE(t2 != nullptr);
    EXPECT_TRUE(cam != nullptr);

    auto shader = instance->CreateShader(renderer->HlslPSCode);
    auto material = std::make_shared<altseed::Material>();
    material->SetShader(shader);

    {
        int c = 0;
        for (int x = 0; x < 5; x++) {
            for (int y = 0; y < 5; y++) {
                auto sprite = std::make_shared<altseed::Sprite>();
                sprite->SetMaterial(material);
                sprite->SetPosition(asd::Vector2DF(x * 120, y * 120));
                sprite->SetSize(asd::Vector2DF(80, 80));
                sprite->SetTexture(c++ % 2 == 0 ? t1 : t2);
                renderer->Sprites.push_back(sprite);
            }
        }
    }

    {
        auto sprite = std::make_shared<altseed::Sprite>();
        sprite->SetMaterial(material);
        sprite->SetPosition(asd::Vector2DF(900, 100));
        sprite->SetSize(asd::Vector2DF(256, 256));
        sprite->SetTexture(cam->GetTarget()->GetNativeTexture());
        renderer->Sprites.push_back(sprite);
    }

    while (count++ < 100 && instance->DoEvents()) EXPECT_TRUE(instance->Update());

    altseed::Graphics::Terminate();
}