// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphicPanel.h"

#include "HorizontalComboBox.h"
#include "G/G.h"
#include "GameFramework/GameUserSettings.h"

#define LOCTEXT_NAMESPACE "Arkadia"

bool UGraphicPanel::Initialize()
{
	bool res = Super::Initialize();

	FScreenResolutionArray screen_resolutions;

	if (IsValid(resolution))
	{
		resolution->ClearDefaultOptions();
	}

	if (RHIGetAvailableResolutions(screen_resolutions, true))
	{		
		for (FScreenResolutionRHI& screen_resolution : screen_resolutions)
		{
			if (screen_resolution.Width < 1280)
			{
				continue;
			}

			float aspect_ratio = static_cast<float>(screen_resolution.Width) / static_cast<float>(screen_resolution.Height);

			float aspect_ratio_16_9 = 16.f/9.f;
			float aspect_ratio_21_9 = 21.f/9.f;

			float tolerance = 0.01f;

			if(!(FMath::Abs(aspect_ratio - aspect_ratio_16_9) < tolerance ||
				FMath::Abs(aspect_ratio - aspect_ratio_21_9) < tolerance))
			{
				continue;
			}
			
			if (IsValid(resolution))
			{
				resolution->AddDefaultOption(
					FText::Format(LOCTEXT("GraphicPanel", "{0}x{1}"), screen_resolution.Width,
					              screen_resolution.Height));
			}
		}
	}

	UGameUserSettings* user_settings = GEngine->GetGameUserSettings();

	if (user_settings)
	{
		FIntPoint set_resolution = user_settings->GetScreenResolution();
		if (IsValid(resolution))
		{
			resolution->SetSelectedOption(
				FText::Format(LOCTEXT("GraphicPanel", "{0}x{1}"), set_resolution.X, set_resolution.Y));
		}

		EWindowMode::Type set_window_mode = user_settings->GetFullscreenMode();
		
		if (IsValid(window_mode))
		{
			window_mode->SetSelectedOption(ConvertWindowMode(set_window_mode));
		}

		int32 set_aa_quality = user_settings->GetAntiAliasingQuality();
		if (IsValid(anti_aliasing))
		{
			anti_aliasing->SetSelectedOption(set_aa_quality);
		}

		int32 set_texture_quality = user_settings->GetTextureQuality();
		if (IsValid(texture_quality))
		{
			texture_quality->SetSelectedOption(set_texture_quality);
		}

		int32 set_shadow_quality = user_settings->GetShadowQuality();
		if (IsValid(shadow_quality))
		{
			shadow_quality->SetSelectedOption(set_shadow_quality);
		}

		int32 set_effect_quality = user_settings->GetVisualEffectQuality();
		if(IsValid(effect_quality))
		{
			effect_quality->SetSelectedOption(set_effect_quality);
		}

		int32 set_shading_quality = user_settings->GetShadingQuality();
		if(IsValid(shading_quality))
		{
			shading_quality->SetSelectedOption(set_shading_quality);
		}

		int32 set_foliage_quality = user_settings->GetFoliageQuality();
		if(IsValid(foliage_quality))
		{
			foliage_quality->SetSelectedOption(set_foliage_quality);
		}
		
		int32 set_pp_quality = user_settings->GetPostProcessingQuality();
		if (IsValid(post_processing))
		{
			post_processing->SetSelectedOption(set_pp_quality);
		}

		int32 set_view_distance = user_settings->GetViewDistanceQuality();
		if (IsValid(view_distance))
		{
			view_distance->SetSelectedOption(set_view_distance);
		}

		float set_frame_rate_limit = user_settings->GetFrameRateLimit();
		if (IsValid(fps_limit))
		{
			fps_limit->SetSelectedOption(ConvertFPS(set_frame_rate_limit));
		}
	}

	Apply();
	
	return res;
}

EWindowMode::Type UGraphicPanel::ConvertWindowMode(FText mode)
{
	if (mode.EqualTo(LOCTEXT("GraphicPanel", "전체화면")))
	{
		return EWindowMode::Fullscreen;
	}

	if (mode.EqualTo(LOCTEXT("GraphicPanel", "창모드")))
	{
		return EWindowMode::Windowed;
	}

	if (mode.EqualTo(LOCTEXT("GraphicPanel", "경계없는 창모드")))
	{
		return EWindowMode::WindowedFullscreen;
	}

	return EWindowMode::Fullscreen;
}

FText UGraphicPanel::ConvertWindowMode(EWindowMode::Type mode)
{
	if (mode == EWindowMode::Fullscreen)
	{
		return FText(LOCTEXT("GraphicPanel", "전체화면"));
	}

	if (mode == EWindowMode::Windowed)
	{
		return FText(LOCTEXT("GraphicPanel", "창모드"));
	}

	if (mode == EWindowMode::WindowedFullscreen)
	{
		return FText(LOCTEXT("GraphicPanel", "경계없는 창모드"));
	}

	return FText(LOCTEXT("GraphicPanel", "전체화면"));
}

float UGraphicPanel::ConvertFPS(FText fps)
{
	return FCString::Atof(*fps.ToString());
}

FText UGraphicPanel::ConvertFPS(float fps)
{
	return FText::FromString(FString::FromInt(static_cast<int>(fps)));
}

FIntPoint UGraphicPanel::ConvertResolution(FText _resolution)
{
	LOG(Log, "%s", *_resolution.ToString())
	FString left_s, right_s;
	FString in_s = "x";

	_resolution.ToString().Split(in_s, &left_s, &right_s);

	left_s = left_s.Replace(TEXT(","), TEXT(""));
	right_s = right_s.Replace(TEXT(","), TEXT(""));

	return FIntPoint{FCString::Atoi(*left_s), FCString::Atoi(*right_s)};
}

void UGraphicPanel::On()
{
	Super::On();
}

void UGraphicPanel::Off()
{
	Super::Off();
}

void UGraphicPanel::Apply()
{
	UGameUserSettings* setting = GEngine->GetGameUserSettings();

	setting->SetFullscreenMode(ConvertWindowMode(window_mode->GetSelectedOption()));
	setting->SetFrameRateLimit(ConvertFPS(fps_limit->GetSelectedOption()));
	setting->SetScreenResolution(ConvertResolution(resolution->GetSelectedOption()));
	setting->SetViewDistanceQuality(view_distance->GetSelectedOptionIdx());
	setting->SetPostProcessingQuality(post_processing->GetSelectedOptionIdx());
	setting->SetAntiAliasingQuality(anti_aliasing->GetSelectedOptionIdx());
	setting->SetTextureQuality(texture_quality->GetSelectedOptionIdx());
	setting->SetShadowQuality(shadow_quality->GetSelectedOptionIdx());
	setting->SetShadingQuality(shading_quality->GetSelectedOptionIdx());
	setting->SetVisualEffectQuality(effect_quality->GetSelectedOptionIdx());
	setting->SetFoliageQuality(foliage_quality->GetSelectedOptionIdx());

	window_mode->Apply();
	fps_limit->Apply();
	resolution->Apply();
	view_distance->Apply();
	post_processing->Apply();
	anti_aliasing->Apply();
	texture_quality->Apply();
	shadow_quality->Apply();
	shading_quality->Apply();
	effect_quality->Apply();
	foliage_quality->Apply();

	setting->ApplySettings(false);
	setting->SaveSettings();
}

FIntPoint UGraphicPanel::GetResolution()
{
	return ConvertResolution(resolution->GetSelectedOption());
}

#undef LOCTEXT_NAMESPACE
